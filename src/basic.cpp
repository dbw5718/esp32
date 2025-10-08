// basic.cpp

#include <WiFi.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include "basic.h"
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <ESP32Time.h>
#include "draw_scale.h"

// 全局对象定义
ESP32Time rtc(0);

// 全局LED引脚定义
const int led_pin[5] = {2, 4, 5, 18, 19};

// --------------- 定义 char 数组 ---------------
char city[20] = {0}; // 初始化数组，全部为0
char weather_condition[20] = {0};
char weekday[20] = {0};

// 数值变量定义
int temperature = 0;
int humidity = 0;
int year = 0, month = 0, day = 0;
int hour = 0, minute = 0, second = 0;

// LED函数（无需修改）
void led_init(void) { /* ... 保持不变 ... */ }
void led_mode(int mode) { /* ... 保持不变 ... */ }

// --------------- 修改 attain_weather 函数 ---------------
void attain_weather(const char *url)
{
    HTTPClient http;
    if (WiFi.status() == WL_CONNECTED)
    {
        http.begin(url);
        int httpCode = http.GET();

        if (httpCode == HTTP_CODE_OK)
        {
            String response = http.getString();
            JsonDocument doc;
            DeserializationError error = deserializeJson(doc, response);
            if (error)
            {
                Serial.print("JSON解析失败: ");
                Serial.println(error.c_str());
                http.end();
                return;
            }

            // 使用 strncpy 将 String 内容复制到 char 数组
            // doc["..."].as<String>().c_str() 可以获取 String 的 const char* 指针
            // sizeof(city) - 1 是为了留出一个字节给 '\0'
            strncpy(city, doc["lives"][0]["city"].as<String>().c_str(), sizeof(city) - 1);
            city[sizeof(city) - 1] = '\0'; // 手动确保字符串以'\0'结尾

            strncpy(weather_condition, doc["lives"][0]["weather"].as<String>().c_str(), sizeof(weather_condition) - 1);
            weather_condition[sizeof(weather_condition) - 1] = '\0';

            // 温度和湿度直接用 as<int>() 转换，因为它们是 int 类型
            temperature = doc["lives"][0]["temperature"].as<int>();
            humidity = doc["lives"][0]["humidity"].as<int>();

            // 调试打印
            Serial.printf("城市: %s\n", city);
            Serial.printf("天气: %s\n", weather_condition);
            Serial.printf("温度: %d\n", temperature);
            Serial.printf("湿度: %d\n", humidity);
        }
        http.end();
    }
}

// --------------- 修改 attain_time 函数 ---------------
void attain_time(const char *url)
{
    HTTPClient http;
    if (WiFi.status() == WL_CONNECTED)
    {
        http.begin(url);
        int httpCode = http.GET();

        if (httpCode == HTTP_CODE_OK)
        {
            String response = http.getString();
            JsonDocument doc;
            DeserializationError error = deserializeJson(doc, response);
            if (error)
            {
                Serial.print("JSON解析失败: ");
                Serial.println(error.c_str());
                http.end();
                return;
            }

            // 星期几同样用 strncpy
            strncpy(weekday, doc["weekday"].as<String>().c_str(), sizeof(weekday) - 1);
            weekday[sizeof(weekday) - 1] = '\0';

            String date_information = doc["date"].as<String>();
            // 日期时间的解析逻辑不变，因为它们直接赋值给 int
            year = date_information.substring(0, 4).toInt();
            month = date_information.substring(5, 7).toInt();
            day = date_information.substring(8, 10).toInt();
            hour = date_information.substring(11, 13).toInt();
            minute = date_information.substring(14, 16).toInt();
            second = date_information.substring(17, 19).toInt();

            // 调试打印
            Serial.printf("日期: %d-%02d-%02d\n", year, month, day);
            Serial.printf("星期: %s\n", weekday);
            Serial.printf("时间: %02d:%02d:%02d\n", hour, minute, second);
        }
        http.end();
    }
}