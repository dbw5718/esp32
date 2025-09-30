#include "draw_scale.h"
#include <cstring>
#include "my_chinese_font_16.h"
#include "weather_icons.h"

// 全局变量（可选，方便后续切换图标）
lv_obj_t *ui_weather_img; // 图片控件，用于显示天气图标


/**
 * 创建UI布局（极简分区，强制居中）
 */
void create_weather_ui(void)
{
    // 2. 获取当前屏幕（LVGL显示控件需要挂载到屏幕上）
    lv_obj_t *scr = lv_scr_act();

    // 3. 创建「图片控件」（代替原来的标签控件，因为要显示图片而非文字）
    ui_weather_img = lv_img_create(scr);

    // 4. 设置图片来源：加载你整合的图片图标（这里以Sunny为例，可替换为Cloudy、Rainy等）
    lv_img_set_src(ui_weather_img, &Sunny); // 关键：&图标名，对应weather_icons.h中的声明

    // 5. 设置图标位置（示例：屏幕中心偏上50px，可根据需求调整）
    lv_obj_align(ui_weather_img, LV_ALIGN_CENTER, 0, -50); // 最后两个参数是X/Y偏移量

    // 6. （可选）设置图标大小（如果图标是64x64，不设置也会默认显示原尺寸，无需额外配置）
    // lv_img_set_size(ui_weather_img, 64, 64); // 宽64，高64，与你的图标尺寸匹配

    // （可选功能：3秒后自动切换为多云图标，验证多图标是否正常）
    lv_timer_create([](lv_timer_t *timer) {
        lv_img_set_src(ui_weather_img, &Cloudy); // 切换为Cloudy图标
    }, 3000, NULL); // 3000ms=3秒后执行


    
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
