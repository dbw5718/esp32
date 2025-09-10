#include "thermometer.h"

static lv_obj_t *thermometer = NULL;

void create_thermometer() {
    // 创建环形温度计对象
    thermometer = lv_scale_create(lv_screen_active());
    lv_obj_set_size(thermometer, 200, 200); // 温度计大小
    lv_scale_set_label_show(thermometer, true); // 显示温度标签
    lv_scale_set_mode(thermometer, LV_SCALE_MODE_ROUND_OUTER); // 环形外刻度模式
    lv_obj_center(thermometer); // 屏幕居中
    
    // 基础参数配置（温度范围、刻度）
    lv_scale_set_range(thermometer, -20, 40); // 温度范围：-20~40°C
    lv_scale_set_total_tick_count(thermometer, 61); // 总刻度数
    lv_scale_set_major_tick_every(thermometer, 10); // 每10°C一个主刻度
    static const char *temp_labels[] = {"-20°C", "-10°C", "0°C", "10°C", "20°C", "30°C", "40°C", NULL};
    lv_scale_set_text_src(thermometer, temp_labels); // 自定义标签
    
    // 默认样式（0~30°C，绿色区域）
    // 主刻度样式
    lv_obj_set_style_line_color(thermometer, lv_palette_darken(LV_PALETTE_GREEN, 2), LV_PART_INDICATOR);
    lv_obj_set_style_line_width(thermometer, 2, LV_PART_INDICATOR);
    lv_obj_set_style_length(thermometer, 15, LV_PART_INDICATOR);
    
    // 小刻度样式
    lv_obj_set_style_line_color(thermometer, lv_palette_lighten(LV_PALETTE_GREEN, 1), LV_PART_ITEMS);
    lv_obj_set_style_line_width(thermometer, 1, LV_PART_ITEMS);
    lv_obj_set_style_length(thermometer, 8, LV_PART_ITEMS);
    
    // 环形基线样式
    lv_obj_set_style_arc_color(thermometer, lv_palette_darken(LV_PALETTE_GREEN, 2), LV_PART_MAIN);
    lv_obj_set_style_arc_width(thermometer, 3, LV_PART_MAIN);
    
    // 标签样式
    lv_obj_set_style_text_font(thermometer, LV_FONT_DEFAULT, LV_PART_ANY);
    lv_obj_set_style_text_color(thermometer, lv_palette_darken(LV_PALETTE_GREEN, 3), LV_PART_ANY);
    
    // 低温区域（-20~0°C，蓝色区域）
    static lv_style_t cold_indicator;
    static lv_style_t cold_items;
    static lv_style_t cold_main;
    lv_style_init(&cold_indicator);
    lv_style_set_line_color(&cold_indicator, lv_palette_darken(LV_PALETTE_BLUE, 2));
    lv_style_set_line_width(&cold_indicator, 2);
    lv_style_set_length(&cold_indicator, 15);
    
    lv_style_init(&cold_items);
    lv_style_set_line_color(&cold_items, lv_palette_lighten(LV_PALETTE_BLUE, 1));
    lv_style_set_line_width(&cold_items, 1);
    lv_style_set_length(&cold_items, 8);
    
    lv_style_init(&cold_main);
    lv_style_set_arc_color(&cold_main, lv_palette_darken(LV_PALETTE_BLUE, 2));
    lv_style_set_arc_width(&cold_main, 3);
    
    // 添加低温分段并应用样式
    lv_scale_section_t *cold_sec = lv_scale_add_section(thermometer);
    lv_scale_set_section_range(thermometer, cold_sec, -20, 0);
    lv_scale_set_section_style_indicator(thermometer, cold_sec, &cold_indicator);
    lv_scale_set_section_style_items(thermometer, cold_sec, &cold_items);
    lv_scale_set_section_style_main(thermometer, cold_sec, &cold_main);
    
    // 高温区域（30~40°C，红色区域）
    static lv_style_t hot_indicator;
    static lv_style_t hot_items;
    static lv_style_t hot_main;
    lv_style_init(&hot_indicator);
    lv_style_set_line_color(&hot_indicator, lv_palette_darken(LV_PALETTE_RED, 2));
    lv_style_set_line_width(&hot_indicator, 2);
    lv_style_set_length(&hot_indicator, 15);
    
    lv_style_init(&hot_items);
    lv_style_set_line_color(&hot_items, lv_palette_lighten(LV_PALETTE_RED, 1));
    lv_style_set_line_width(&hot_items, 1);
    lv_style_set_length(&hot_items, 8);
    
    lv_style_init(&hot_main);
    lv_style_set_arc_color(&hot_main, lv_palette_darken(LV_PALETTE_RED, 2));
    lv_style_set_arc_width(&hot_main, 3);
    
    // 添加高温分段并应用样式
    lv_scale_section_t *hot_sec = lv_scale_add_section(thermometer);
    lv_scale_set_section_range(thermometer, hot_sec, 30, 40);
    lv_scale_set_section_style_indicator(thermometer, hot_sec, &hot_indicator);
    lv_scale_set_section_style_items(thermometer, hot_sec, &hot_items);
    lv_scale_set_section_style_main(thermometer, hot_sec, &hot_main);
    
    // 指针样式
    static lv_style_t pointer_style;
    lv_style_init(&pointer_style);
    lv_style_set_line_color(&pointer_style, lv_palette_darken(LV_PALETTE_GREY, 3));
    lv_style_set_line_width(&pointer_style, 3);
    lv_style_set_length(&pointer_style, 20);
    lv_obj_add_style(thermometer, &pointer_style, LV_PART_KNOB);
    
    // 设置初始温度（25°C，带动画过渡）
    set_temperature(25);
}

void set_temperature(int temp, bool anim) {
    if (thermometer) {
        lv_bar_set_value(thermometer, temp, anim ? LV_ANIM_ON : LV_ANIM_OFF);
    }
}
