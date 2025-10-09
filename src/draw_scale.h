#ifndef DRAW_SCALE_H
#define DRAW_SCALE_H

#include "lvgl/lvgl.h"
#include <Arduino.h>
#include "basic.h"

// 全局UI控件指针（仅保留必要控件，删除无用的ui_weather_icon_label等）
extern lv_obj_t *city_label;    // 城市标签
extern lv_obj_t *weather_img;   // 天气图标（图片控件）
extern lv_obj_t *week_label;    // 星期/日期标签
extern lv_obj_t *time_label;    // 时间标签
extern lv_obj_t *tem_hum_label; // 温湿度标签

// 函数声明（参数类型：const char*对应char数组，int对应数值）
void create_weather_ui(void);                  // 创建所有UI（仅执行一次）
void update_weather(const char *weather_cond); // 更新天气图标（接收char数组）
void update_week(const char *weekday);         // 更新星期/日期（接收char数组）
void update_time(int hour, int minute);        // 更新时间（接收int数值）
void update_tem_hum(int temp, int humi);       // 更新温湿度（接收int数值）

#endif // DRAW_SCALE_H