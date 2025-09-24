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

// 全局UI组件
lv_obj_t * city_label;
lv_obj_t * weather_icon;
lv_obj_t * weather_label;
lv_obj_t * time_label;
lv_obj_t * date_label;
lv_obj_t * temp_label;
lv_obj_t * humi_label;

// 天气类型枚举（用于switch判断）
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

// 创建UI布局（保持不变）
void create_weather_ui(void) {
    lv_obj_t * screen = lv_screen_active();
    lv_obj_clean(screen);

    // 顶部区域：城市 + 天气图标 + 描述
    lv_obj_t * top_cont = lv_obj_create(screen);
    lv_obj_set_size(top_cont, 240, 60);
    lv_obj_set_align(top_cont, LV_ALIGN_TOP_MID);
    lv_obj_remove_style_all(top_cont);

    city_label = lv_label_create(top_cont);
    lv_label_set_text(city_label, "北京市");
    lv_obj_set_style_text_font(city_label, &lv_font_montserrat_16, 0);
    lv_obj_set_align(city_label, LV_ALIGN_LEFT_MID);
    lv_obj_set_x(city_label, 10);

    weather_icon = lv_label_create(top_cont);
    lv_label_set_text(weather_icon, "☀️");
    lv_obj_set_style_text_font(weather_icon, &lv_font_montserrat_24, 0);
    lv_obj_set_align(weather_icon, LV_ALIGN_RIGHT_MID);
    lv_obj_set_x(weather_icon, -60);

    weather_label = lv_label_create(top_cont);
    lv_label_set_text(weather_label, "晴");
    lv_obj_set_style_text_font(weather_label, &lv_font_montserrat_14, 0);
    lv_obj_align_to(weather_label, weather_icon, LV_ALIGN_OUT_RIGHT_MID, 5, 0);

    // 中间区域：时间 + 日期
    lv_obj_t * mid_cont = lv_obj_create(screen);
    lv_obj_set_size(mid_cont, 240, 120);
    lv_obj_set_align(mid_cont, LV_ALIGN_CENTER);
    lv_obj_remove_style_all(mid_cont);

    time_label = lv_label_create(mid_cont);
    lv_label_set_text(time_label, "09:45");
    lv_obj_set_style_text_font(time_label, &lv_font_montserrat_48, 0);
    lv_obj_set_align(time_label, LV_ALIGN_TOP_MID);

    date_label = lv_label_create(mid_cont);
    lv_label_set_text(date_label, "2024年9月24日 周三");
    lv_obj_set_style_text_font(date_label, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(date_label, lv_color_hex(0x666666), 0);
    lv_obj_align_to(date_label, time_label, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);

    // 底部区域：温度 + 湿度
    lv_obj_t * bottom_cont = lv_obj_create(screen);
    lv_obj_set_size(bottom_cont, 240, 60);
    lv_obj_set_align(bottom_cont, LV_ALIGN_BOTTOM_MID);
    lv_obj_remove_style_all(bottom_cont);

    // 温度模块
    lv_obj_t * temp_cont = lv_obj_create(bottom_cont);
    lv_obj_set_size(temp_cont, 120, 60);
    lv_obj_set_align(temp_cont, LV_ALIGN_LEFT_MID);
    lv_obj_remove_style_all(temp_cont);

    lv_obj_t * temp_icon = lv_label_create(temp_cont);
    lv_label_set_text(temp_icon, "🌡️");
    lv_obj_set_style_text_font(temp_icon, &lv_font_montserrat_20, 0);
    lv_obj_set_align(temp_icon, LV_ALIGN_LEFT_MID);
    lv_obj_set_x(temp_icon, 15);

    temp_label = lv_label_create(temp_cont);
    lv_label_set_text(temp_label, "25℃");
    lv_obj_set_style_text_font(temp_label, &lv_font_montserrat_24, 0);
    lv_obj_align_to(temp_label, temp_icon, LV_ALIGN_OUT_RIGHT_MID, 8, 0);

    // 湿度模块
    lv_obj_t * humi_cont = lv_obj_create(bottom_cont);
    lv_obj_set_size(humi_cont, 120, 60);
    lv_obj_set_align(humi_cont, LV_ALIGN_RIGHT_MID);
    lv_obj_remove_style_all(humi_cont);

    lv_obj_t * humi_icon = lv_label_create(humi_cont);
    lv_label_set_text(humi_icon, "💧");
    lv_obj_set_style_text_font(humi_icon, &lv_font_montserrat_20, 0);
    lv_obj_set_align(humi_icon, LV_ALIGN_LEFT_MID);
    lv_obj_set_x(humi_icon, 15);

    humi_label = lv_label_create(humi_cont);
    lv_label_set_text(humi_label, "45%RH");
    lv_obj_set_style_text_font(humi_label, &lv_font_montserrat_24, 0);
    lv_obj_align_to(humi_label, humi_icon, LV_ALIGN_OUT_RIGHT_MID, 8, 0);
}

/**
 * 将天气文字转换为枚举类型（用于switch判断）
 * @param weather 天气描述字符串
 * @return 对应的天气枚举值
 */
static weather_type_t get_weather_type(const char * weather) {
    if (strstr(weather, "晴") && !strstr(weather, "云") && !strstr(weather, "夜")) {
        return WEATHER_SUNNY;
    }
    else if (strstr(weather, "晴") && strstr(weather, "云") && !strstr(weather, "夜")) {
        return WEATHER_PARTLY_CLOUDY;
    }
    else if ((strstr(weather, "阴") || strstr(weather, "云")) && !strstr(weather, "夜")) {
        return WEATHER_CLOUDY;
    }
    else if (strstr(weather, "雨") && strstr(weather, "小") && !strstr(weather, "雷")) {
        return WEATHER_LIGHT_RAIN;
    }
    else if (strstr(weather, "雨") && !strstr(weather, "小") && !strstr(weather, "雷")) {
        return WEATHER_HEAVY_RAIN;
    }
    else if (strstr(weather, "雷") && strstr(weather, "雨")) {
        return WEATHER_THUNDERSTORM;
    }
    else if (strstr(weather, "雪") && strstr(weather, "小")) {
        return WEATHER_LIGHT_SNOW;
    }
    else if (strstr(weather, "雪") && !strstr(weather, "小")) {
        return WEATHER_HEAVY_SNOW;
    }
    else if (strstr(weather, "雾")) {
        return WEATHER_FOG;
    }
    else if (strstr(weather, "风")) {
        return WEATHER_WINDY;
    }
    else if (strstr(weather, "夜") && strstr(weather, "晴")) {
        return WEATHER_NIGHT_CLEAR;
    }
    else if (strstr(weather, "夜") && strstr(weather, "云")) {
        return WEATHER_NIGHT_CLOUDY;
    }
    else {
        return WEATHER_UNKNOWN;
    }
}

/**
 * 使用switch语句更新天气图标
 */
void update_weather_data(const char * city, const char * weather, 
                         const char * time, const char * date,
                         const char * temp, const char * humi) {
    // 更新基础文本信息
    lv_label_set_text(city_label, city);
    lv_label_set_text(weather_label, weather);
    lv_label_set_text(time_label, time);
    lv_label_set_text(date_label, date);
    lv_label_set_text(temp_label, temp);
    lv_label_set_text(humi_label, humi);

    // 获取天气类型枚举值
    weather_type_t type = get_weather_type(weather);

    // 使用switch语句更新图标（核心优化点）
    switch (type) {
        case WEATHER_SUNNY:
            lv_label_set_text(weather_icon, "☀️");
            break;
        case WEATHER_PARTLY_CLOUDY:
            lv_label_set_text(weather_icon, "⛅");
            break;
        case WEATHER_CLOUDY:
            lv_label_set_text(weather_icon, "☁️");
            break;
        case WEATHER_LIGHT_RAIN:
            lv_label_set_text(weather_icon, "🌧️");
            break;
        case WEATHER_HEAVY_RAIN:
            lv_label_set_text(weather_icon, "🌨️");
            break;
        case WEATHER_THUNDERSTORM:
            lv_label_set_text(weather_icon, "⛈️");
            break;
        case WEATHER_LIGHT_SNOW:
            lv_label_set_text(weather_icon, "❄️");
            break;
        case WEATHER_HEAVY_SNOW:
            lv_label_set_text(weather_icon, "❅");
            break;
        case WEATHER_FOG:
            lv_label_set_text(weather_icon, "🌫️");
            break;
        case WEATHER_WINDY:
            lv_label_set_text(weather_icon, "💨");
            break;
        case WEATHER_NIGHT_CLEAR:
            lv_label_set_text(weather_icon, "🌙");
            break;
        case WEATHER_NIGHT_CLOUDY:
            lv_label_set_text(weather_icon, "🌥️");
            break;
        case WEATHER_UNKNOWN:
        default:
            lv_label_set_text(weather_icon, "?");  // 未知天气显示问号
            break;
    }
}