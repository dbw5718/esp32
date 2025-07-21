#include <WiFi.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include "basic.h"
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <RTClib.h>
#include <ESP32Time.h>
#include "zh_front_20.h"



//编写函数
  void led_init(void)
  {
    for(int i=0;i<5;i++)
    {
      pinMode(led_pin[i],OUTPUT);
      digitalWrite(led_pin[i], HIGH);
    }
  }

  void led_mode(int mode)          //共阳极
  {
    if(mode==1)
    {
      for(int i=0;i<5;i++)
      {
        digitalWrite(led_pin[i], LOW);
        for(int j=0;j<5&&j!=i;j++)
        {
          digitalWrite(led_pin[j], HIGH);   
        }
        delay(200);
      }
      
      for(int i=0;i<5;i++)
      {
        digitalWrite(led_pin[i], HIGH);   
      }
      
    }
    // else if(mode==2)
    // {
    // digitalWrite(LED1, HIGH);
    // digitalWrite(LED2, HIGH);
    // digitalWrite(LED3, HIGH);
    // digitalWrite(LED4, HIGH);
    // digitalWrite(LED5, HIGH);
    // }
   
    
  }




String city;
String weather_condition;
String winddirection;
int tem;

void attain_weather(String url)
{
   HTTPClient http;
   if (WiFi.status() == WL_CONNECTED)
    {
      // 设置目标网站的URL
      http.begin(url);

      // 发起GET请求并等待响应
      int httpCode = http.GET();
      
        if (httpCode > 0) 
        {
          if (httpCode == HTTP_CODE_OK)
          {
            // 获取响应的内容
            String response = http.getString();
            DynamicJsonDocument doc(1024);
            DeserializationError error = deserializeJson(doc, response);

            city = doc["lives"][0]["city"].as<String>();
            weather_condition = doc["lives"][0]["weather"].as<String>();
            tem = doc["lives"][0]["temperature"];
            winddirection = doc["lives"][0]["winddirection"].as<String>();
          }
        }
          
    }
}

String date_information;
String date;
int hour=0,minute=0,second=0;
int year=2023,month=5,day=2;
int code=0;

void attain_time(String url)
{
HTTPClient http;
   if (WiFi.status() == WL_CONNECTED)
    {
      // 设置目标网站的URL
      http.begin(url);

      // 发起GET请求并等待响应
      int httpCode = http.GET();

        if (httpCode > 0) 
        {
          if (httpCode == HTTP_CODE_OK)
          {
            // 获取响应的内容
            String response = http.getString();
            DynamicJsonDocument doc(1024);
            DeserializationError error = deserializeJson(doc, response);

            date_information = doc["dateTime"].as<String>();
            
            date = date_information.substring(0,10);
            year = date_information.substring(0,4).toInt();
            month = date_information.substring(5,7).toInt();
            day = date_information.substring(8,10).toInt();
            hour = date_information.substring(11,13).toInt();
            minute = date_information.substring(14,16).toInt();
            second = date_information.substring(17,19).toInt();
            
          }
        }
          
    }
}