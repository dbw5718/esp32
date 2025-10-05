#ifndef DRAW_SCALE_H
#define DRAW_SCALE_H

#include "lvgl/lvgl.h"
#include <Arduino.h>
#include "basic.h"


// 全局UI组件声明
extern lv_obj_t * city_label;
extern lv_obj_t * weather_icon;
extern lv_obj_t * weather_label;
extern lv_obj_t * time_label;
extern lv_obj_t * date_label;
extern lv_obj_t * temp_label;
extern lv_obj_t * humi_label;
extern lv_obj_t * ui_weather_icon_label;

// 函数声明
void create_weather_ui(void);
void update_weather(String weather_condition);
void update_week(String weekday);
void update_time(String hour,String minute);
void update_tem_hum(String temperature,String humidity);
                         

#endif // DRAW_SCALE_H