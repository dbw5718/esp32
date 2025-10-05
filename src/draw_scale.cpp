#include "draw_scale.h"
#include <cstring>
#include "my_chinese_font_16.h"
#include "weather_icons.h"
#include <Arduino.h>
#include <ESP32Time.h>
#include "basic.h"

// 全局变量（可选，方便后续切换图标）
lv_obj_t *ui_weather_img; // 图片控件，用于显示天气图标
char time_buf[32];
char week_buf[32];
char tem_buf[32];

/**
 * 创建UI布局（极简分区，强制居中）
 */
void create_weather_ui(void)
{
    lv_obj_t *label1 = lv_label_create(lv_screen_active());
    lv_label_set_long_mode(label1, LV_LABEL_LONG_MODE_WRAP); /*Break the long lines*/
    lv_label_set_recolor(label1, true);                      /*Enable re-coloring by commands in the text*/
    lv_label_set_text(label1, "#0000ff Linyi");
    lv_obj_set_style_text_font(label1, &lv_font_montserrat_16, 0);
    lv_obj_set_width(label1, 150); /*Set smaller width to make the lines wrap*/
    lv_obj_set_style_text_align(label1, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(label1, LV_ALIGN_CENTER, -80, -100);

    update_weather(weather_condition);
    update_week(weekday);
}

void update_weather(String weather_condition)
{
    // 1. 中文天气转英文图标名（用之前的映射逻辑）
    const char *icon_name = NULL;
    if (weather_condition == "晴")
        icon_name = "Sunny";
    else if (weather_condition == "多云")
        icon_name = "Partly_Cloudy";
    else if (weather_condition == "阴")
        icon_name = "Overcast";
    else if (weather_condition == "小雨")
        icon_name = "Light_Rain";
    else if (weather_condition == "中雨")
        icon_name = "Moderate_Rain";
    else if (weather_condition == "大雨")
        icon_name = "Heavy_Rain";
    else if (weather_condition == "小雪")
        icon_name = "Light_Snow";
    else if (weather_condition == "大雪")
        icon_name = "Heavy_Snow";
    else if (weather_condition == "雾")
        icon_name = "Foggy";
    else
        icon_name = "Unknown"; // 未匹配时用默认图标（需确保有Unknown图标）

    // 2. 获取当前屏幕（LVGL显示控件需要挂载到屏幕上）
    lv_obj_t *scr = lv_scr_act();
    // 3. 创建「图片控件」（代替原来的标签控件，因为要显示图片而非文字）
    ui_weather_img = lv_img_create(scr);

    // 2. 根据图标名显示对应图片（关键：通过指针匹配图标变量）
    if (icon_name == "Sunny")
        lv_img_set_src(ui_weather_img, &Sunny);
    else if (icon_name == "Partly_Cloudy")
        lv_img_set_src(ui_weather_img, &Partly_Cloudy);
    else if (icon_name == "Overcast")
        lv_img_set_src(ui_weather_img, &Overcast);
    else if (icon_name == "Light_Rain")
        lv_img_set_src(ui_weather_img, &Light_Rain);
    else if (icon_name == "Moderate_Rain")
        lv_img_set_src(ui_weather_img, &Moderate_Rain);
    else if (icon_name == "Heavy_Rain")
        lv_img_set_src(ui_weather_img, &Heavy_Rain);
    else if (icon_name == "Light_Snow")
        lv_img_set_src(ui_weather_img, &Light_Snow);
    else if (icon_name == "Heavy_Snow")
        lv_img_set_src(ui_weather_img, &Heavy_Snow);
    else if (icon_name == "Foggy")
        lv_img_set_src(ui_weather_img, &Foggy);
    else
        lv_img_set_src(ui_weather_img, &unknow); // 默认图标

    lv_obj_align(ui_weather_img, LV_ALIGN_CENTER, 70, -80); // 最后两个参数是X/Y偏移量
}

void update_week(String weekday)
{

    const char *week_name = NULL;
    if (weekday == "星期一")
        week_name = "Mon.";
    else if (weekday == "星期二")
        week_name = "Tue.";
    else if (weekday == "星期三")
        week_name = "Wed.";
    else if (weekday == "星期四")
        week_name = "Thu.";
    else if (weekday == "星期五")
        week_name = "Fri.";
    else if (weekday == "星期六")
        week_name = "Sat.";
    else if (weekday == "星期日")
        week_name = "Sun.";
    else
        week_name = "Unknown"; // 未匹配时用默认图标（需确保有Unknown图标）

    // --- 1. 创建并设置 "日期时间" 标签 ---
    lv_obj_t *label_time = lv_label_create(lv_screen_active());
    lv_label_set_long_mode(label_time, LV_LABEL_LONG_MODE_SCROLL_CIRCULAR);
    lv_label_set_recolor(label_time, true);

    snprintf(week_buf, sizeof(week_buf), "#0000ff %04d-%02d-%02d %04s", year, month, day, week_name);

    lv_label_set_text(label_time, week_buf);
    lv_obj_set_style_text_font(label_time, &lv_font_montserrat_16, 0);
    lv_obj_set_width(label_time, 150);
    lv_obj_set_style_text_align(label_time, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(label_time, LV_ALIGN_CENTER, 0, 20); // 位置在上
}

void update_time(String hour, String minute)
{

    lv_obj_t *label_time1 = lv_label_create(lv_screen_active());
    lv_label_set_long_mode(label_time1, LV_LABEL_LONG_MODE_SCROLL_CIRCULAR);
    lv_label_set_recolor(label_time1, true);
    snprintf(time_buf, sizeof(time_buf), "#0000ff %02d:%02d", hour, minute);
    lv_label_set_text(label_time1, time_buf);
    lv_obj_set_style_text_font(label_time1, &lv_font_montserrat_48, 0);
    lv_obj_set_width(label_time1, 150);
    lv_obj_set_style_text_align(label_time1, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(label_time1, LV_ALIGN_CENTER, 0, -20); // 位置在上
}

void update_tem_hum(String temperature, String humidity)
{

    // --- 3. 创建并设置 "温度湿度" 标签 ---
    lv_obj_t *label_temp_hum = lv_label_create(lv_screen_active());
    lv_label_set_long_mode(label_temp_hum, LV_LABEL_LONG_MODE_WRAP);
    lv_label_set_recolor(label_temp_hum, true);
    snprintf(tem_buf, sizeof(tem_buf), "#0000ff %02d°C / 02%d%", temperature, humidity);

    lv_label_set_text(label_temp_hum, tem_buf);
    lv_obj_set_style_text_font(label_temp_hum, &lv_font_montserrat_16, 0);
    lv_obj_set_width(label_temp_hum, 150);
    lv_obj_set_style_text_align(label_temp_hum, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(label_temp_hum, LV_ALIGN_CENTER, 0, 80); // 位置在下
}