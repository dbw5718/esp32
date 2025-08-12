#include <WiFi.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include "basic.h"
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "zh_front_20.h"
#include <RTClib.h>
#include <ESP32Time.h>
#include "draw.h"
#include <lvgl.h>



//定义两个字符串指针常量
const char* name="301";            //wifi名称
const char* passwd="Chrome1314...@";       //wifi密码


  void setup()  
  {

    led_init();
    tft.init(); 
    tft.fillScreen(TFT_WHITE);
    tft.setRotation(1);
    tft.setTextColor(TFT_RED);
    tft.setTextSize(1);
    tft.setCursor(0,0,2);

    Serial.begin(9600);

    WiFi.begin(name,passwd); 

    tft.print("wifi connecting");
    while(WiFi.status()!=WL_CONNECTED)      //未连接时阻塞程序，直到连接成功
    {           
      delay(500);
      if(tft.getCursorX()>=145&&tft.getCursorY()>=16)
      {
        tft.fillScreen(TFT_WHITE);//清空屏幕
        tft.setCursor(0,0,2);
        tft.print("wifi connecting");
      }
      tft.print(".");
      Serial.println(tft.getCursorY());
      
    }

    tft.fillScreen(TFT_WHITE);                  //清空屏幕
    tft.setCursor(0,0,2);
    tft.println("wifi connected !");
    tft.setCursor(5,20,2);
    tft.print("ip :  "); 
    tft.setTextColor(TFT_RED);
    tft.print(WiFi.localIP()); 
    delay(1000);
    tft.fillScreen(TFT_WHITE);

    tft.setCursor(5,5,2);
    tft.print("Getting information...");
    attain_weather("https://restapi.amap.com/v3/weather/weatherInfo?city=371300&key=6a86d8c29d5163008cba8bac08a09cdd");  //获取天气信息
    attain_time("https://cn.apihz.cn/api/time/getapi.php?id=88888888&key=88888888&type=2");     //获取时间信息
  

    WiFi.mode(WIFI_OFF);           //获取信息后断开网络减小开销

    rtc.setTime(second, minute, hour, day, month, year);  //将获取到的时间信息写入内部时钟
    
    tft.fillScreen(TFT_WHITE);

    // tft.drawLine(0,50,240,50,TFT_BLACK);
    // tft.drawLine(50,0,50,50,TFT_BLACK);
    // tft.drawLine(150,0,150,50,TFT_BLACK);
    // tft.drawLine(0,100,240,100,TFT_BLACK);

    tft.setTextColor(TFT_DARKGREY);
    
    time_show(hour, minute, second); //显示时间
    // Serial.println(hour);
    // Serial.println(minute);
    // Serial.println(second);

    //weather_show(city,weather_condition,winddirection,tem); //显示天气信息
  }
  
  void loop()
  {
    time_show(rtc.getTime("%H").toInt(),rtc.getTime("%M").toInt(),rtc.getTime("%S").toInt()); 
    astronaut_show();
    delay(100);

  }
  

  
  


