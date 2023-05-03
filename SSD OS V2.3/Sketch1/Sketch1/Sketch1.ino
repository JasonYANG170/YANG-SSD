/*
 Name:		YANG SSD OS.ino
 Created:	2023/5/3 19:24:00
 Author:	qw200
*/
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

Adafruit_SH1106G display = Adafruit_SH1106G(128, 64, &Wire);

Adafruit_AHTX0 aht;

void setup() {
  Serial.begin(115200);
  //while (!Serial);


  Serial.println("128x64 OLED FeatherWing test");
  display.begin(0x3C, true); // Address 0x3C default

  Serial.println("OLED begun");

  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.


  // Clear the buffer.
  display.clearDisplay();


  display.setRotation(0);

  if (aht.begin()) {
    Serial.println("Found AHT20");
  } else {
    Serial.println("Didn't find AHT20");
  }  

  display.setTextSize(5);
  display.setTextColor(SH110X_WHITE);  
      display.print("YANG ");

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
      WiFi.mode(WIFI_STA); // ����ģʽ��espĬ��ΪSTA+AP
    WiFiManager wm;  //����manager����
    bool res;
    res = wm.autoConnect("Flowerpot","12345678"); //����ap���ƺ�����


  display.setTextSize(1);
}
String asdsd;
void loop() {
    net("https://api.vvhan.com/api/en?type=sj");
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
void net(String url) {
    if (WiFi.status() == WL_CONNECTED) {
        WiFiClientSecure client;
        client.setInsecure(); // Do not verify certificate
        HTTPClient https;
        https.begin(client, url);
        int httpCode = https.GET();
        if (httpCode > 0) {
            String payload = https.getString();
            Serial.println(payload);
            Serial.println("");
            // �ص�1������������json�ļ�
            String json = payload;
            Serial.println(String("") + "JSON��С��" + json.length());
            // �ص�2��������JSON���ݴ�С
            DynamicJsonDocument doc(json.length() * 2); //������JSON���ݴ�С


            // �ص�3�������л�����
            deserializeJson(doc, json);

            // �ص�4����ȡ�������������Ϣ
            String nameStr = doc["data"]["en"].as<String>();
            // int numberInt = doc["time"].as<int>();
            asdsd = nameStr;
            // ͨ�����ڼ���������������������Ϣ
            Serial.print("errorStr = "); Serial.println(nameStr);
            //Serial.print("errnoInt = ");Serial.println(numberInt);

        }
        https.end();
    }
}