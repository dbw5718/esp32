#include <WiFi.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include "basic.h"
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "zh_front_20.h"
#include <RTClib.h>
#include <ESP32Time.h>
#include "mpu.h"



//定义两个字符串指针常量
const char* id="301";            //wifi名称
const char* psw="Chrome...1314@";       //wifi密码

IMU imu;  //实例化IMU对象

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

    WiFi.begin(id,psw); 

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
    attain_time("https://timeapi.io/api/Time/current/zone?timeZone=Asia/Shanghai");     //获取时间信息

    WiFi.mode(WIFI_OFF);           //获取信息后断开网络减小开销

    rtc.setTime(second, minute, hour, day, month, year);  //将获取到的时间信息写入内部时钟
    
    tft.fillScreen(TFT_WHITE);

    tft.drawLine(0,25,160,25,TFT_BLACK);
    tft.drawLine(50,0,50,25,TFT_BLACK);
    tft.drawLine(115,0,115,25,TFT_BLACK);
    tft.drawLine(0,85,160,85,TFT_BLACK);

    tft.setTextColor(TFT_DARKGREY);
    //写入天气部分
    tft.loadFont(font_20);            
    tft.drawString(city.substring(0, 6), 5,5);
    if(weather_condition.length()==6)              //天气状况居中显示
    {
      tft.drawString(weather_condition,62,tft.getCursorY());
    }
    else if(weather_condition.length()==3)
    {
      tft.drawString(weather_condition,72,tft.getCursorY());
    }
    else
    {
      tft.drawString(weather_condition,50,tft.getCursorY());
    }
    

    tft.unloadFont();
    tft.setCursor( 120,tft.getCursorY(),2);
    tft.print(tem);
    tft.loadFont(font_20);
    tft.drawString("℃", tft.getCursorX(),tft.getCursorY());
    tft.unloadFont();
    
    //写入日期部分
    tft.setTextColor(TFT_RED);
    tft.setCursor(20,100,2);     
    tft.print(date);


    imu.init();  //初始化MPU6050传感器

    
    
  }
  
  void loop()
  {
    time_show(rtc.getTime("%H").toInt(),rtc.getTime("%M").toInt(),rtc.getTime("%S").toInt()); 
    astronaut_show();

    imu.update();  //更新传感器数据

    // display tab-separated accel/gyro x/y/z values
    Serial.print("a/g/t:\t");
    Serial.print(imu.getAccelX()); Serial.print("\t");
    Serial.print(imu.getAccelY()); Serial.print("\t");
    Serial.print(imu.getAccelZ()); Serial.print("\t");
    Serial.print(imu.getGyroX()); Serial.print("\t");
    Serial.print(imu.getGyroY()); Serial.print("\t");
    Serial.print(imu.getGyroZ()); Serial.print("\t");
    Serial.println(imu.getTemperature());

    delay(100);

  }
  

  
  


