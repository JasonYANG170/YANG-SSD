/*
 Name:		YANG SSD OS.ino
 Created:	2023/5/3 19:24:00
 Author:	JasonYANG17
*/
//导入所需库
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <Adafruit_AHTX0.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include <WiFiManager.h>
//配置像素及颜色
Adafruit_SH1106G display = Adafruit_SH1106G(128, 64, &Wire);
//配置AHT驱动
Adafruit_AHTX0 aht;

void setup() {
//配置串口调试频率
  Serial.begin(115200);
  //while (!Serial);


  Serial.println("128x64 OLED FeatherWing test");
//配置iic地址
  display.begin(0x3C, true); // Address 0x3C default

  Serial.println("OLED begun");

  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.


  // Clear the buffer.
//清屏
  display.clearDisplay();

//设置光标位置（屏幕坐标点）
  display.setRotation(0);
//检查AHT20是否工作
  if (aht.begin()) {
    Serial.println("Found AHT20");
  } else {
    Serial.println("Didn't find AHT20");
  }  
//配置文本大小
  display.setTextSize(5);
//配置颜色
  display.setTextColor(SH110X_WHITE);  
//配置输出文本
      display.print("YANG ");
//显示输出
     display.display();
   
        display.setTextSize(2);
  delay(5000);
      display.clearDisplay();
    display.setRotation(0);
           display.setTextColor(SH110X_WHITE);  
             display.setCursor(0,0);
                 display.println(" YANG SSD");
                 display.println("");
      display.print("Connecting to WiFi...");
      
     display.display();
//配置wifi模式
      WiFi.mode(WIFI_STA); // 设置模式，esp默认为STA+AP
//定义对象
    WiFiManager wm;  //定义manager对象
    bool res;
    res = wm.autoConnect("Flowerpot","12345678"); //定义ap名称和密码（bool型）


  display.setTextSize(1);
}
String asdsd;
void loop() {
    net("https://api.vvhan.com/api/en?type=sj");//调用webapi函数
delay(5000);
    display.clearDisplay();
    display.setRotation(0);
         display.setTextSize(1);
           display.setTextColor(SH110X_WHITE);  
             display.setCursor(0,0);
             sensors_event_t humidity, temp;
  
  aht.getEvent(&humidity, &temp);// populate temp and humidity objects with fresh data
                     display.println("       YANG SSD"); 
                       display.println("");
  display.print("Temp: "); display.print(temp.temperature); display.println(" C");
  display.print("Hum: "); display.print(humidity.relative_humidity); display.println(" %");
          display.println("");
    display.println(asdsd); 
       display.display();
}
//webapi函数
void net(String url) {
    if (WiFi.status() == WL_CONNECTED) {//确认网络是否连接
        WiFiClientSecure client;//配置网络客户机
        client.setInsecure(); // Do not verify certificate配置为不安全的网络认证
        HTTPClient https;//配置为Https代理
        https.begin(client, url);//发送客户机和URL请求
        int httpCode = https.GET();//获取HttpGet请求返回值
        if (httpCode > 0) {//返回值大于0代表请求成功
            String payload = https.getString();//获取Get请求返回文本
            Serial.println(payload);/输出返回文本
            Serial.println("");
            // 重点1：即将解析的json文件
            String json = payload;
            Serial.println(String("") + "JSON大小：" + json.length());
            // 重点2：解析的JSON数据大小
            DynamicJsonDocument doc(json.length() * 2); //解析的JSON数据大小


            // 重点3：反序列化数据
            deserializeJson(doc, json);

            // 重点4：获取解析后的数据信息
            String nameStr = doc["data"]["en"].as<String>();
            // int numberInt = doc["time"].as<int>();
            asdsd = nameStr;
            // 通过串口监视器输出解析后的数据信息
            Serial.print("errorStr = "); Serial.println(nameStr);
            //Serial.print("errnoInt = ");Serial.println(numberInt);

        }
        https.end();
    }
}