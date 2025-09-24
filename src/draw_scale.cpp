#include "draw_scale.h"
#include <Arduino.h>
#include "zh_font_20.h"

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

// 天气类型枚举（保持不变）
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

// 创建UI布局（修复布局问题）
void create_weather_ui(void) {
    lv_obj_t * screen = lv_screen_active();
    lv_obj_clean(screen);
    // 给屏幕设置垂直Flex布局，让三个容器（上/中/下）垂直排列
    lv_obj_set_layout(screen, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(screen, LV_FLEX_FLOW_COLUMN);  // 垂直方向排列
    lv_obj_set_flex_align(screen, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_SPACE_EVENLY);  // 均匀分布
    lv_obj_set_size(screen, 240, 240);  // 强制屏幕尺寸为240x240（避免自适应错误）

    /*************************
     1. 顶部区域：城市 + 天气图标 + 描述（水平排列）
     *************************/
    lv_obj_t * top_cont = lv_obj_create(screen);
    lv_obj_set_size(top_cont, 240, 60);  // 固定高度60px
    lv_obj_remove_style_all(top_cont);
    // 顶部容器启用水平Flex布局，子组件从左到右排列
    lv_obj_set_layout(top_cont, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(top_cont, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(top_cont, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_SPACE_AROUND);  // 均匀分布

    // 城市标签（左）
    city_label = lv_label_create(top_cont);
    lv_label_set_text(city_label, "北京市");
    lv_obj_set_style_text_font(city_label, &zh_font_20, 0);

    // 天气图标（中）
    weather_icon = lv_label_create(top_cont);
    lv_label_set_text(weather_icon, "☀️");
    lv_obj_set_style_text_font(weather_icon, &zh_font_20, 0);

    // 天气描述（右）
    weather_label = lv_label_create(top_cont);
    lv_label_set_text(weather_label, "晴");
    lv_obj_set_style_text_font(weather_label, &zh_font_20, 0);

    /*************************
     2. 中间区域：时间 + 日期（垂直排列）
     *************************/
    lv_obj_t * mid_cont = lv_obj_create(screen);
    lv_obj_set_size(mid_cont, 240, 120);  // 固定高度120px
    lv_obj_remove_style_all(mid_cont);
    // 中间容器启用垂直Flex布局，子组件从上到下排列
    lv_obj_set_layout(mid_cont, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(mid_cont, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(mid_cont, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);  // 居中对齐

    // 时间标签（上）
    time_label = lv_label_create(mid_cont);
    lv_label_set_text(time_label, "09:45");
    lv_obj_set_style_text_font(time_label, &lv_font_montserrat_48, 0);

    // 日期标签（下，跟在时间下方）
    date_label = lv_label_create(mid_cont);
    lv_label_set_text(date_label, "2024年9月24日 周三");
    lv_obj_set_style_text_font(date_label, &zh_font_20, 0);
    lv_obj_set_style_text_color(date_label, lv_color_hex(0x666666), 0);
    lv_obj_set_style_margin_top(date_label, 10, 0);  // 与时间保持10px间距

    /*************************
     3. 底部区域：温度 + 湿度（水平排列）
     *************************/
    lv_obj_t * bottom_cont = lv_obj_create(screen);
    lv_obj_set_size(bottom_cont, 240, 60);  // 固定高度60px
    lv_obj_remove_style_all(bottom_cont);
    // 底部容器启用水平Flex布局，子组件从左到右排列
    lv_obj_set_layout(bottom_cont, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(bottom_cont, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(bottom_cont, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_SPACE_AROUND);  // 均匀分布

    // 温度模块（左：图标+文本）
    lv_obj_t * temp_cont = lv_obj_create(bottom_cont);
    lv_obj_remove_style_all(temp_cont);
    lv_obj_set_layout(temp_cont, LV_LAYOUT_FLEX);  // 子组件水平排列
    lv_obj_set_flex_flow(temp_cont, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(temp_cont, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    lv_obj_t * temp_icon = lv_label_create(temp_cont);
    lv_label_set_text(temp_icon, "🌡️");
    lv_obj_set_style_text_font(temp_icon, &zh_font_20, 0);
    lv_obj_set_style_margin_right(temp_icon, 8, 0);  // 图标与文本间距8px

    temp_label = lv_label_create(temp_cont);
    lv_label_set_text(temp_label, "25℃");
    lv_obj_set_style_text_font(temp_label, &zh_font_20, 0);

    // 湿度模块（右：图标+文本）
    lv_obj_t * humi_cont = lv_obj_create(bottom_cont);
    lv_obj_remove_style_all(humi_cont);
    lv_obj_set_layout(humi_cont, LV_LAYOUT_FLEX);  // 子组件水平排列
    lv_obj_set_flex_flow(humi_cont, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(humi_cont, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    lv_obj_t * humi_icon = lv_label_create(humi_cont);
    lv_label_set_text(humi_icon, "💧");
    lv_obj_set_style_text_font(humi_icon, &zh_font_20, 0);
    lv_obj_set_style_margin_right(humi_icon, 8, 0);  // 图标与文本间距8px

    humi_label = lv_label_create(humi_cont);
    lv_label_set_text(humi_label, "45%RH");
    lv_obj_set_style_text_font(humi_label, &zh_font_20, 0);
}

// get_weather_type 函数（保持不变）
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

// update_weather_data 函数（保持不变）
void update_weather_data(const char * city, const char * weather, 
                         const char * time, const char * date,
                         const char * temp, const char * humi) {
    lv_label_set_text(city_label, city);
    lv_label_set_text(weather_label, weather);
    lv_label_set_text(time_label, time);
    lv_label_set_text(date_label, date);
    lv_label_set_text(temp_label, temp);
    lv_label_set_text(humi_label, humi);

    weather_type_t type = get_weather_type(weather);
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
            lv_label_set_text(weather_icon, "?");
            break;
    }
}