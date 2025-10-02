#include "draw_scale.h"
#include <cstring>
#include "my_chinese_font_16.h"
#include "weather_icons.h"
#include <Arduino.h>

// 全局变量（可选，方便后续切换图标）
lv_obj_t *ui_weather_img; // 图片控件，用于显示天气图标


/**
 * 创建UI布局（极简分区，强制居中）
 */
void create_weather_ui(void)
{
    lv_obj_t *label1 = lv_label_create(lv_screen_active());
    lv_label_set_long_mode(label1, LV_LABEL_LONG_MODE_WRAP); /*Break the long lines*/
    lv_label_set_recolor(label1, true);                      /*Enable re-coloring by commands in the text*/
    lv_label_set_text(label1, "#0000ff Beijing");
    lv_obj_set_style_text_font(label1, &lv_font_montserrat_16, 0);
    lv_obj_set_width(label1, 150); /*Set smaller width to make the lines wrap*/
    lv_obj_set_style_text_align(label1, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(label1, LV_ALIGN_CENTER, -80, -100);

    // --- 1. 创建并设置 "日期时间" 标签 ---
    lv_obj_t *label_time1 = lv_label_create(lv_screen_active());
    lv_label_set_long_mode(label_time1, LV_LABEL_LONG_MODE_SCROLL_CIRCULAR);
    lv_label_set_recolor(label_time1, true);
    lv_label_set_text(label_time1, "#0000ff 15:30");
    lv_obj_set_style_text_font(label_time1, &lv_font_montserrat_48, 0);
    lv_obj_set_width(label_time1, 150);
    lv_obj_set_style_text_align(label_time1, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(label_time1, LV_ALIGN_CENTER, 0, -20); // 位置在上

    // --- 1. 创建并设置 "日期时间" 标签 ---
    lv_obj_t *label_time = lv_label_create(lv_screen_active());
    lv_label_set_long_mode(label_time, LV_LABEL_LONG_MODE_SCROLL_CIRCULAR);
    lv_label_set_recolor(label_time, true);
    lv_label_set_text(label_time, "#0000ff 2023-10-27 Sat");
    lv_obj_set_style_text_font(label_time, &lv_font_montserrat_16, 0);
    lv_obj_set_width(label_time, 150);
    lv_obj_set_style_text_align(label_time, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(label_time, LV_ALIGN_CENTER, 0, 20); // 位置在上

    // --- 3. 创建并设置 "温度湿度" 标签 ---
    lv_obj_t *label_temp_hum = lv_label_create(lv_screen_active());
    lv_label_set_long_mode(label_temp_hum, LV_LABEL_LONG_MODE_WRAP);
    lv_label_set_recolor(label_temp_hum, true);
    lv_label_set_text(label_temp_hum, "#0000ff 26°C / 45%");
    lv_obj_set_style_text_font(label_temp_hum, &lv_font_montserrat_16, 0);
    lv_obj_set_width(label_temp_hum, 150);
    lv_obj_set_style_text_align(label_temp_hum, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(label_temp_hum, LV_ALIGN_CENTER, 0, 80); // 位置在下
    update_weather(weather_condition);
}

void update_weather(String weather_condition)
{


    // 1. 中文天气转英文图标名（用之前的映射逻辑）
    const char *icon_name = NULL;
    if (weather_condition == "晴") icon_name = "Sunny";
    else if (weather_condition == "多云") icon_name = "Partly_Cloudy";
    else if (weather_condition == "阴") icon_name = "Overcast";
    else if (weather_condition == "小雨") icon_name = "Light_Rain";
    else if (weather_condition == "中雨") icon_name = "Moderate_Rain";
    else if (weather_condition == "大雨") icon_name = "Heavy_Rain";
    else if (weather_condition == "小雪") icon_name = "Light_Snow";
    else if (weather_condition == "大雪") icon_name = "Heavy_Snow";
    else if (weather_condition == "雾") icon_name = "Foggy";
    else icon_name = "Unknown"; // 未匹配时用默认图标（需确保有Unknown图标）

     // 2. 获取当前屏幕（LVGL显示控件需要挂载到屏幕上）
    lv_obj_t *scr = lv_scr_act();
    // 3. 创建「图片控件」（代替原来的标签控件，因为要显示图片而非文字）
    ui_weather_img = lv_img_create(scr);

    // 2. 根据图标名显示对应图片（关键：通过指针匹配图标变量）
    if (icon_name == "Sunny") lv_img_set_src(ui_weather_img, &Sunny);
    else if (icon_name == "Partly_Cloudy") lv_img_set_src(ui_weather_img, &Partly_Cloudy);
    else if (icon_name == "Overcast") lv_img_set_src(ui_weather_img, &Overcast);
    else if (icon_name == "Light_Rain") lv_img_set_src(ui_weather_img, &Light_Rain);
    else if (icon_name == "Moderate_Rain") lv_img_set_src(ui_weather_img, &Moderate_Rain);
    else if (icon_name == "Heavy_Rain") lv_img_set_src(ui_weather_img, &Heavy_Rain);
    else if (icon_name == "Light_Snow") lv_img_set_src(ui_weather_img, &Light_Snow);
    else if (icon_name == "Heavy_Snow") lv_img_set_src(ui_weather_img, &Heavy_Snow);
    else if (icon_name == "Foggy") lv_img_set_src(ui_weather_img, &Foggy);
    else lv_img_set_src(ui_weather_img, &unknow); // 默认图标
    
    lv_obj_align(ui_weather_img, LV_ALIGN_CENTER, 70, -80); // 最后两个参数是X/Y偏移量
}
