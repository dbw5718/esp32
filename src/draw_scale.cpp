#include "draw_scale.h"
#include <cstring>
#include "my_chinese_font_16.h"

/**
 * 创建UI布局（极简分区，强制居中）
 */
void create_weather_ui(void) {


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

    // --- 创建一个标签来显示天气图标 ---
    lv_obj_t * weather_icon = lv_label_create(lv_screen_active());
    
    // 1. 设置要显示的图标
    // LV_SYMBOL_SUN 是一个宏，定义了太阳图标的 Unicode 编码
    lv_label_set_text(weather_icon, LV_SYMBOL_);

    // 2. 设置字体为 Material Icons
    // 必须使用 lv_font_material_icons_xx 系列的字体，图标才能正确显示
    // 后面的数字代表字体大小，你可以根据需要选择 16, 24, 32, 48 等
    lv_obj_set_style_text_font(weather_icon, &lv_font_material_icons_48, 0);

    // 3. 设置图标颜色
    lv_obj_set_style_text_color(weather_icon, lv_color_hex(0xFFFF00), 0); // 设置为黄色

    // 4. 定位图标
    lv_obj_align(weather_icon, LV_ALIGN_CENTER, -50, -20);


    // --- 创建一个标签来显示天气文字 ---
    lv_obj_t * weather_text = lv_label_create(lv_screen_active());
    lv_label_set_text(weather_text, "sun");
    lv_obj_set_style_text_font(weather_text, &my_chinese_font_16, 0); // 使用一个中文字体
    lv_obj_align_to(weather_text, weather_icon, LV_ALIGN_OUT_RIGHT_MID, 10, 0);


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

void create_test(void){
    
}
