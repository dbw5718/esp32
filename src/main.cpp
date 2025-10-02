#include <lvgl.h>
#include <TFT_eSPI.h>
#include <Arduino.h>
#include "display.h"
#include "draw_scale.h" // 确保包含LVGL刻度组件头文件
#include "basic.h"
#include <WiFi.h>

// 定义两个字符串指针常量
const char *name = "ZTE-KyNNUG";              // wifi名称
const char *passwd = "zxcvbnm1234"; // wifi密码

void setup()
{
    Serial.begin(115200);
    WiFi.begin(name,passwd);
    while (WiFi.status() != WL_CONNECTED) // 未连接时阻塞程序，直到连接成功
    {
        delay(500);
        Serial.println("trying connect....");
    }
    display_init();                                                                                                     // 初始化显示设备
    attain_weather("https://restapi.amap.com/v3/weather/weatherInfo?city=371300&key=6a86d8c29d5163008cba8bac08a09cdd"); // 获取天气信息
    attain_time("https://api.uuni.cn//api/time");
    create_weather_ui(); // 创建天气UI布局
}

void loop()
{
    lv_timer_handler(); // LVGL主循环（必须高频调用）
    delay(5);           // 短延迟，不阻塞UI
}