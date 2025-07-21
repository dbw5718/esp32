#ifndef _basic_H
#define _basic_H
#include <ESP32Time.h>
#include <TFT_eSPI.h>
#define FRAME_NUM 13

const char led_pin[8]={15,2,4,16,17};
//函数声明
void led_init(void);           //拉低所有led引脚的电平
void led_mode(int mode);       
String parseHTML(String html, String startTag, String endTag);
String extractDataFromHTML(String html, String tag, String attribute);
void attain_weather(String url);
void attain_time(String url);


extern TFT_eSPI tft;
extern ESP32Time rtc;
extern String city;
extern String weather_condition;
extern String winddirection;
extern int tem;
extern String date_information;
extern String date;
extern int hour,minute,second;
extern int year,month,day;


#endif
