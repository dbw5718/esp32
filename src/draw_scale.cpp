#include "draw_scale.h"
#include <Arduino.h>

// void create_scale() {
//     Serial.println("创建环形刻度（兼容模式）...");
//     lv_obj_t * scale = lv_scale_create(lv_screen_active());
//     lv_obj_set_size(scale, 150, 150);
//     lv_scale_set_label_show(scale, true);
//     lv_scale_set_mode(scale, LV_SCALE_MODE_ROUND_OUTER);
//     lv_obj_center(scale);

//     lv_scale_set_total_tick_count(scale, 21);
//     lv_scale_set_major_tick_every(scale, 5);

//     lv_obj_set_style_length(scale, 5, LV_PART_ITEMS);
//     lv_obj_set_style_length(scale, 10, LV_PART_INDICATOR);
//     lv_scale_set_range(scale, 0, 100);

//     static const char * custom_labels[] = {"0 °C", "25 °C", "50 °C", "75 °C", "100 °C", NULL};
//     lv_scale_set_text_src(scale, custom_labels);

//     static lv_style_t indicator_style;
//     lv_style_init(&indicator_style);

//     /* Label style properties */
//     lv_style_set_text_font(&indicator_style, LV_FONT_DEFAULT);
//     lv_style_set_text_color(&indicator_style, lv_palette_darken(LV_PALETTE_BLUE, 3));

//     /* Major tick properties */
//     lv_style_set_line_color(&indicator_style, lv_palette_darken(LV_PALETTE_BLUE, 3));
//     lv_style_set_width(&indicator_style, 10U);      /*Tick length*/
//     lv_style_set_line_width(&indicator_style, 2U);  /*Tick width*/
//     lv_obj_add_style(scale, &indicator_style, LV_PART_INDICATOR);

//     static lv_style_t minor_ticks_style;
//     lv_style_init(&minor_ticks_style);
//     lv_style_set_line_color(&minor_ticks_style, lv_palette_lighten(LV_PALETTE_BLUE, 2));
//     lv_style_set_width(&minor_ticks_style, 5U);         /*Tick length*/
//     lv_style_set_line_width(&minor_ticks_style, 2U);    /*Tick width*/
//     lv_obj_add_style(scale, &minor_ticks_style, LV_PART_ITEMS);

//     static lv_style_t main_line_style;
//     lv_style_init(&main_line_style);
//     /* Main line properties */
//     lv_style_set_arc_color(&main_line_style, lv_palette_darken(LV_PALETTE_BLUE, 3));
//     lv_style_set_arc_width(&main_line_style, 2U); /*Tick width*/
//     lv_obj_add_style(scale, &main_line_style, LV_PART_MAIN);

//     /* Add a section */
//     static lv_style_t section_minor_tick_style;
//     static lv_style_t section_label_style;
//     static lv_style_t section_main_line_style;

//     lv_style_init(&section_label_style);
//     lv_style_init(&section_minor_tick_style);
//     lv_style_init(&section_main_line_style);

//     /* Label style properties */
//     lv_style_set_text_font(&section_label_style, LV_FONT_DEFAULT);
//     lv_style_set_text_color(&section_label_style, lv_palette_darken(LV_PALETTE_RED, 3));

//     lv_style_set_line_color(&section_label_style, lv_palette_darken(LV_PALETTE_RED, 3));
//     lv_style_set_line_width(&section_label_style, 5U); /*Tick width*/

//     lv_style_set_line_color(&section_minor_tick_style, lv_palette_lighten(LV_PALETTE_RED, 2));
//     lv_style_set_line_width(&section_minor_tick_style, 4U); /*Tick width*/

//     /* Main line properties */
//     lv_style_set_arc_color(&section_main_line_style, lv_palette_darken(LV_PALETTE_RED, 3));
//     lv_style_set_arc_width(&section_main_line_style, 4U); /*Tick width*/

//     /* Configure section styles */
//     lv_scale_section_t * section = lv_scale_add_section(scale);
//     lv_scale_set_section_range(scale, section, 75, 100);
//     lv_scale_set_section_style_indicator(scale, section, &section_label_style);
//     lv_scale_set_section_style_items(scale, section, &section_minor_tick_style);
//     lv_scale_set_section_style_main(scale, section, &section_main_line_style);


//     Serial.println("环形刻度创建完成");
// }

// 全局变量（用于后续更新数据）
lv_obj_t * city_label;    // 城市
lv_obj_t * weather_icon;  // 天气图标
lv_obj_t * weather_label; // 天气文字
lv_obj_t * time_label;    // 时间
lv_obj_t * date_label;    // 日期
lv_obj_t * temp_label;    // 温度
lv_obj_t * humi_label;    // 湿度

// 创建布局的函数
void create_weather_ui(void) {
    lv_obj_t * screen = lv_screen_active();
    lv_obj_clean(screen); // 清空当前屏幕

    /* -------------------------- 1. 顶部区（城市+天气） -------------------------- */
    lv_obj_t * top_cont = lv_obj_create(screen);
    lv_obj_set_size(top_cont, 240, 60); // 宽240，高60
    lv_obj_set_align(top_cont, LV_ALIGN_TOP_MID); // 顶部居中
    lv_obj_remove_style_all(top_cont); // 移除默认样式（无背景/边框）

    // 城市标签
    city_label = lv_label_create(top_cont);
    lv_label_set_text(city_label, "北京");
    lv_obj_set_style_text_font(city_label, &lv_font_montserrat_14, 0); // 使用现有14px字体
    lv_obj_set_align(city_label, LV_ALIGN_LEFT_MID); // 顶部区左侧居中
    lv_obj_set_x(city_label, 10); // 左偏移10px

    // 天气图标（用UTF-8字符替代）
    weather_icon = lv_label_create(top_cont);
    lv_label_set_text(weather_icon, "☀️"); // 晴天图标（UTF-8）
    lv_obj_set_style_text_font(weather_icon, &lv_font_montserrat_14, 0); // 用现有字体
    lv_obj_set_align(weather_icon, LV_ALIGN_RIGHT_MID); // 顶部区右侧居中
    lv_obj_set_x(weather_icon, -40); // 右偏移-40px（给文字留空间）

    weather_label = lv_label_create(top_cont);
    lv_label_set_text(weather_label, "多云");
    lv_obj_set_style_text_font(weather_label, &lv_font_montserrat_14, 0);
    lv_obj_align_to(weather_label, weather_icon, LV_ALIGN_OUT_RIGHT_MID, 5, 0); // 图标右侧5px


    /* -------------------------- 2. 中间区（时间+日期） -------------------------- */
    lv_obj_t * mid_cont = lv_obj_create(screen);
    lv_obj_set_size(mid_cont, 240, 120);
    lv_obj_set_align(mid_cont, LV_ALIGN_CENTER);
    lv_obj_remove_style_all(mid_cont);

    // 时间标签（核心）
    time_label = lv_label_create(mid_cont);
    lv_label_set_text(time_label, "14:35");
    lv_obj_set_style_text_font(time_label, &lv_font_montserrat_14, 0); // 用现有字体（若有更大字体可替换）
    lv_obj_set_align(time_label, LV_ALIGN_TOP_MID); // 中间区顶部居中

    // 日期标签（辅助）
    date_label = lv_label_create(mid_cont);
    lv_label_set_text(date_label, "2024-05-20");
    lv_obj_set_style_text_font(date_label, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(date_label, lv_color_hex(0x888888), 0); // 灰色
    lv_obj_align_to(date_label, time_label, LV_ALIGN_OUT_BOTTOM_MID, 0, 10); // 时间下方10px


    /* -------------------------- 3. 底部区（温度+湿度） -------------------------- */
    lv_obj_t * bottom_cont = lv_obj_create(screen);
    lv_obj_set_size(bottom_cont, 240, 60);
    lv_obj_set_align(bottom_cont, LV_ALIGN_BOTTOM_MID);
    lv_obj_remove_style_all(bottom_cont);

    // 温度：图标+文字（UTF-8替代）
    lv_obj_t * temp_cont = lv_obj_create(bottom_cont);
    lv_obj_set_size(temp_cont, 120, 60);
    lv_obj_set_align(temp_cont, LV_ALIGN_LEFT_MID);
    lv_obj_remove_style_all(temp_cont);

    lv_obj_t * temp_icon = lv_label_create(temp_cont);
    lv_label_set_text(temp_icon, "🌡️"); // 温度图标（UTF-8）
    lv_obj_set_style_text_font(temp_icon, &lv_font_montserrat_14, 0);
    lv_obj_set_align(temp_icon, LV_ALIGN_LEFT_MID);
    lv_obj_set_x(temp_icon, 15);

    temp_label = lv_label_create(temp_cont);
    lv_label_set_text(temp_label, "23℃");
    lv_obj_set_style_text_font(temp_label, &lv_font_montserrat_14, 0);
    lv_obj_align_to(temp_label, temp_icon, LV_ALIGN_OUT_RIGHT_MID, 10, 0);

    // 湿度：图标+文字（UTF-8替代）
    lv_obj_t * humi_cont = lv_obj_create(bottom_cont);
    lv_obj_set_size(humi_cont, 120, 60);
    lv_obj_set_align(humi_cont, LV_ALIGN_RIGHT_MID);
    lv_obj_remove_style_all(humi_cont);

    lv_obj_t * humi_icon = lv_label_create(humi_cont);
    lv_label_set_text(humi_icon, "💧"); // 湿度图标（UTF-8）
    lv_obj_set_style_text_font(humi_icon, &lv_font_montserrat_14, 0);
    lv_obj_set_align(humi_icon, LV_ALIGN_LEFT_MID);
    lv_obj_set_x(humi_icon, 15);

    humi_label = lv_label_create(humi_cont);
    lv_label_set_text(humi_label, "58%RH");
    lv_obj_set_style_text_font(humi_label, &lv_font_montserrat_14, 0);
    lv_obj_align_to(humi_label, humi_icon, LV_ALIGN_OUT_RIGHT_MID, 10, 0);
}

// 更新数据的函数（使用UTF-8图标）
void update_weather_data(const char * city, const char * weather, 
                         const char * time, const char * date,
                         const char * temp, const char * humi) {
    lv_label_set_text(city_label, city);
    lv_label_set_text(weather_label, weather);
    
    // 根据天气更新UTF-8图标
    if(strcmp(weather, "晴天") == 0) lv_label_set_text(weather_icon, "☀️");
    else if(strcmp(weather, "多云") == 0) lv_label_set_text(weather_icon, "☁️");
    else if(strcmp(weather, "雨天") == 0) lv_label_set_text(weather_icon, "🌧️");
    else if(strcmp(weather, "雪天") == 0) lv_label_set_text(weather_icon, "❄️");
    
    lv_label_set_text(time_label, time);
    lv_label_set_text(date_label, date);
    lv_label_set_text(temp_label, temp);
    lv_label_set_text(humi_label, humi);
}