// basic.h

#ifndef BASIC_H
#define BASIC_H

#include <Arduino.h>
#include <ESP32Time.h>

// 全局对象声明
extern ESP32Time rtc;

// 全局LED引脚声明
extern const int led_pin[5];

// --------------- 将 String 改为 char 数组 ---------------
// 为每个字符串定义足够的长度，并加1以容纳字符串结束符 '\0'
extern char city[20];              // 城市名
extern char weather_condition[20]; // 天气状况
extern char weekday[20];           // 星期几

// 温度和湿度是数值，应该用 int，与 update_tem_hum 函数匹配
extern int temperature;
extern int humidity;

// 时间日期数值用 int
extern int year, month, day;
extern int hour, minute, second;

// 函数声明
void led_init(void);
void led_mode(int mode);
void attain_weather(const char *url); // 参数也建议用 const char*
void attain_time(const char *url);    // 参数也建议用 const char*

#endif // BASIC_H