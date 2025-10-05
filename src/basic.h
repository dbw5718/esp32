#ifndef BASIC_H
#define BASIC_H

#include <Arduino.h> // 必须包含，因为用到了String类型
#include <ESP32Time.h>

extern ESP32Time rtc;

// --------------- 1. 全局LED引脚声明（如果其他文件要用到LED引脚，就加这个）---------------
extern const int led_pin[5]; // 对应basic.cpp里的LED引脚数组

// --------------- 2. 全局天气数据声明（关键！包含weekday）---------------
extern String city;              // 城市名（basic.cpp里定义的）
extern String weather_condition; // 天气状况（如“晴”“多云”）
extern String winddirection;     // 风向
extern int tem;                  // 温度（整数）
extern String weekday;           // 星期几（解决draw_scale.cpp报错的核心！）

// --------------- 3. 全局时间数据声明（如果draw_scale.cpp要用到年/月/日，就加这些）---------------
extern String date; // 日期（如“2024-10-16”）
extern int year;    // 年（整数）
extern int month;   // 月（整数）
extern int day;     // 日（整数）
extern int hour;    // 时（整数）
extern int minute;  // 分（整数）
extern int second;  // 秒（整数）
extern int code;    // 你在basic.cpp里定义的code变量

// --------------- 4. 函数声明（basic.cpp里的函数，其他文件要调用就声明）---------------
void led_init(void);             // LED初始化函数
void led_mode(int mode);         // LED模式控制函数
void attain_weather(String url); // 获取天气数据函数
void attain_time(String url);    // 获取时间数据函数

#endif // BASIC_H