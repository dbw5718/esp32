#include <lvgl.h>
#include <TFT_eSPI.h>
#include <Arduino.h>
#include "display.h"
#include "draw_scale.h"
#include "basic.h"
#include <WiFi.h>
#include <ESP32Time.h>

// 1. WiFi配置
const char *WIFI_NAME = "ZTE-KyNNUG";              // WiFi名称
const char *WIFI_PASS = "zxcvbnm1234";             // WiFi密码
const char *WEATHER_URI = "https://restapi.amap.com/v3/weather/weatherInfo?city=371300&key=6a86d8c29d5163008cba8bac08a09cdd";
const char *TIME_URL = "https://api.uuni.cn//api/time";

// 2. 定时刷新配置（毫秒）
#define WEATHER_REFRESH_INTERVAL 30 * 60 * 1000  // 天气：30分钟
#define TIME_REFRESH_INTERVAL    10 * 1000       // 时间：10秒
#define WIFI_RETRY_MAX           5                // WiFi最大重试次数（避免阻塞）

// 3. 全局时间戳变量（记录上次刷新时间）
unsigned long last_weather_refresh = 0;  // 上次天气刷新时间
unsigned long last_time_refresh = 0;     // 上次时间刷新时间

// 4. 辅助函数：WiFi连接（带重试，非阻塞）
bool wifi_connect_with_retry() {
    if (WiFi.status() == WL_CONNECTED) {
        return true;  // 已连接，直接返回
    }

    Serial.println("\n开始连接WiFi...");
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_NAME, WIFI_PASS);
    
    int retry_cnt = 0;
    while (retry_cnt < WIFI_RETRY_MAX) {
        delay(1000);  // 短延时，避免过度阻塞
        if (WiFi.status() == WL_CONNECTED) {
            Serial.printf("WiFi连接成功！IP：%s\n", WiFi.localIP().toString().c_str());
            return true;
        }
        Serial.printf("WiFi连接中...（重试%d/%d）\n", retry_cnt + 1, WIFI_RETRY_MAX);
        retry_cnt++;
    }

    Serial.println("WiFi连接失败，下次刷新再试！");
    return false;
}

void setup()
{
    Serial.begin(115200);
    Serial.println("程序启动！");

    // 初始化WiFi并获取初始数据
    if (wifi_connect_with_retry()) {
        attain_weather(WEATHER_URI);  // 获取初始天气
        attain_time(TIME_URL);        // 获取初始时间
        WiFi.mode(WIFI_OFF);          // 初始数据获取后断网，降低功耗
        Serial.println("初始数据获取完成，已断开WiFi！");
    } else {
        Serial.println("初始WiFi连接失败，UI可能显示默认值！");
    }

    // 初始化显示、RTC、UI
    display_init();                                                                  
    rtc.setTime(second, minute, hour, day, month, year);  // 时间写入RTC
    create_weather_ui();                                  // 创建UI（初始数据已传入）

    // 记录初始刷新时间（避免启动后立即刷新）
    last_weather_refresh = millis();
    last_time_refresh = millis();
}

void loop()
{
    lv_timer_handler();  // LVGL核心：必须高频调用，确保UI响应
    delay(5);            // 极短延时，不阻塞loop（原500ms太长，已改5ms）

    // 获取当前时间戳（用于定时判断）
    unsigned long current_time = millis();

    // -------------------------- 定时刷新天气 --------------------------
    if (current_time - last_weather_refresh >= WEATHER_REFRESH_INTERVAL) {
        Serial.println("\n=== 到点刷新天气 ===");
        if (wifi_connect_with_retry()) {  // 重新连接WiFi
            attain_weather(WEATHER_URI);  // 获取新天气数据
            update_weather(weather_condition);  // 同步更新UI天气图标
            update_tem_hum(temperature, humidity);  // 同步更新UI温湿度
            WiFi.mode(WIFI_OFF);  // 获取后断网
            Serial.println("天气刷新完成，已断开WiFi！");
        }
        last_weather_refresh = current_time;  // 更新上次刷新时间（避免重复触发）
    }

    // -------------------------- 定时刷新时间 --------------------------
    if (current_time - last_time_refresh >= TIME_REFRESH_INTERVAL) {
        // 直接读取RTC时间（无需联网）
        hour = rtc.getHour();
        minute = rtc.getMinute();
        second = rtc.getSecond();  // 可选：秒数也更新（若UI显示秒）
        
        // 每天0点0分同步一次日期（避免RTC日期偏差）
        if (hour == 0 && minute == 0 && second == 0) {
            day = rtc.getDay();
            month = rtc.getMonth();
            year = rtc.getYear();
            update_week(weekday);  // 同步更新UI日期/星期
            Serial.println("日期已同步（每日0点自动更新）！");
        }

        update_time(hour, minute);  // 同步更新UI时间
        last_time_refresh = current_time;  // 更新上次刷新时间
    }
}