#ifndef DRAW_SCALE_H
#define DRAW_SCALE_H

#include "lvgl/lvgl.h"

// 全局UI组件声明
extern lv_obj_t * city_label;
extern lv_obj_t * weather_icon;
extern lv_obj_t * weather_label;
extern lv_obj_t * time_label;
extern lv_obj_t * date_label;
extern lv_obj_t * temp_label;
extern lv_obj_t * humi_label;

// 天气类型枚举
typedef enum {
    WEATHER_SUNNY = 0,         // 晴天
    WEATHER_PARTLY_CLOUDY,     // 晴间多云
    WEATHER_CLOUDY,            // 阴天
    WEATHER_LIGHT_RAIN,        // 小雨
    WEATHER_HEAVY_RAIN,        // 大雨
    WEATHER_THUNDERSTORM,      // 雷阵雨
    WEATHER_LIGHT_SNOW,        // 小雪
    WEATHER_HEAVY_SNOW,        // 大雪
    WEATHER_FOG,               // 雾天
    WEATHER_WINDY,             // 大风
    WEATHER_NIGHT_CLEAR,       // 夜间晴
    WEATHER_NIGHT_CLOUDY,      // 夜间多云
    WEATHER_UNKNOWN            // 未知天气
} weather_type_t;

// 函数声明
void create_weather_ui(void);
void update_weather_data(const char * city, const char * weather, 
                         const char * time, const char * date,
                         const char * temp, const char * humi);

void create_test(void);

#endif // DRAW_SCALE_H