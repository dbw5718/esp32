// src/weather_icons.h

#ifndef WEATHER_ICON_H
#define WEATHER_ICON_H

#include "lvgl.h"

// 1. 声明字体变量（这才是这个文件该做的事！）
//    这个变量定义在 weather_icons.c 文件中
extern const lv_font_t weather_icon_font;

// 2. 图标宏定义（这部分是正确的，保留）
#define LV_SYMBOL_WEATHER_CLOUDY    "\xEF\x80\x82"
#define LV_SYMBOL_WEATHER_SUNNY     "\xEF\x80\x8D"
#define LV_SYMBOL_WEATHER_WIND      "\xEF\x80\x92"
#define LV_SYMBOL_WEATHER_OVERCAST  "\xEF\x80\x93"
#define LV_SYMBOL_WEATHER_FOG       "\xEF\x80\x94"
#define LV_SYMBOL_WEATHER_HEAVY_RAIN "\xEF\x80\x99"
#define LV_SYMBOL_WEATHER_LIGHT_RAIN "\xEF\x80\x9A"
#define LV_SYMBOL_WEATHER_SNOW      "\xEF\x80\x9B"
#define LV_SYMBOL_WEATHER_THUNDER   "\xEF\x80\x9E"
#define LV_SYMBOL_WEATHER_SAND      "\xEF\x81\xA3"

#endif // WEATHER_ICON_H