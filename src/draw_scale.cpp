#include "draw_scale.h"
#include <cstring>
#include "my_chinese_font_16.h"
#include "weather_icons.h"

lv_obj_t * ui_weather_icon_label;

/**
 * 创建UI布局（极简分区，强制居中）
 */
void create_weather_ui(void)
{
    ui_weather_icon_label = lv_label_create(lv_screen_active());
    lv_obj_set_style_text_font(ui_weather_icon_label, &weather_icon_font, 0); // 设置图标字体
    lv_label_set_text(ui_weather_icon_label, LV_SYMBOL_WEATHER_SUNNY);       /*Break the long lines*/
    lv_obj_align(ui_weather_icon_label, LV_ALIGN_CENTER, -80, -100);

    //     lv_obj_t * label1 = lv_label_create(lv_screen_active());
    //     lv_label_set_long_mode(label1, LV_LABEL_LONG_MODE_WRAP);     /*Break the long lines*/
    //     lv_label_set_recolor(label1, true);                      /*Enable re-coloring by commands in the text*/
    //     lv_label_set_text(label1, "#0000ff Beijing");
    //     lv_obj_set_style_text_font(label1,&lv_font_montserrat_16,0);
    //     lv_obj_set_width(label1, 150);  /*Set smaller width to make the lines wrap*/
    //     lv_obj_set_style_text_align(label1, LV_TEXT_ALIGN_CENTER, 0);
    //     lv_obj_align(label1, LV_ALIGN_CENTER, -80, -100);

    //     // --- 1. 创建并设置 "日期时间" 标签 ---
    //     lv_obj_t * label_time1 = lv_label_create(lv_screen_active());
    //     lv_label_set_long_mode(label_time1, LV_LABEL_LONG_MODE_SCROLL_CIRCULAR);
    //     lv_label_set_recolor(label_time1, true);
    //     lv_label_set_text(label_time1, "#0000ff 15:30");
    //     lv_obj_set_style_text_font(label_time1, &lv_font_montserrat_48, 0);
    //     lv_obj_set_width(label_time1, 150);
    //     lv_obj_set_style_text_align(label_time1, LV_TEXT_ALIGN_CENTER, 0);
    //     lv_obj_align(label_time1, LV_ALIGN_CENTER, 0, -20); // 位置在上

    // // --- 1. 创建并设置 "日期时间" 标签 ---
    //     lv_obj_t * label_time = lv_label_create(lv_screen_active());
    //     lv_label_set_long_mode(label_time, LV_LABEL_LONG_MODE_SCROLL_CIRCULAR);
    //     lv_label_set_recolor(label_time, true);
    //     lv_label_set_text(label_time, "#0000ff 2023-10-27 Sat");
    //     lv_obj_set_style_text_font(label_time, &lv_font_montserrat_16, 0);
    //     lv_obj_set_width(label_time, 150);
    //     lv_obj_set_style_text_align(label_time, LV_TEXT_ALIGN_CENTER, 0);
    //     lv_obj_align(label_time, LV_ALIGN_CENTER, 0, 20); // 位置在上

    // // --- 3. 创建并设置 "温度湿度" 标签 ---
    // lv_obj_t * label_temp_hum = lv_label_create(lv_screen_active());
    // lv_label_set_long_mode(label_temp_hum, LV_LABEL_LONG_MODE_WRAP);
    // lv_label_set_recolor(label_temp_hum, true);
    // lv_label_set_text(label_temp_hum, "#0000ff 26°C / 45%");
    // lv_obj_set_style_text_font(label_temp_hum, &lv_font_montserrat_16, 0);
    // lv_obj_set_width(label_temp_hum, 150);
    // lv_obj_set_style_text_align(label_temp_hum, LV_TEXT_ALIGN_CENTER, 0);
    // lv_obj_align(label_temp_hum, LV_ALIGN_CENTER, 0, 80); // 位置在下
}

void create_test(void)
{
}
