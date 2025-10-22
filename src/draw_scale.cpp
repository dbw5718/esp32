#include "draw_scale.h"
#include <cstring>
#include "weather_icons.h"      // 确保图标文件路径正确
#include <Arduino.h>
#include "basic.h"

// 全局UI控件定义（与.h声明一一对应）
lv_obj_t *city_label;
lv_obj_t *weather_img;
lv_obj_t *week_label;
lv_obj_t *time_label;
lv_obj_t *tem_hum_label;

// 临时缓冲区（仅用于拼接字符串，避免重复定义）
static char time_buf[32] = {0};    // 时间拼接缓冲区
static char week_buf[32] = {0};    // 星期/日期拼接缓冲区
static char tem_hum_buf[32] = {0}; // 温湿度拼接缓冲区

/**
 * 创建UI布局（仅初始化时执行一次，所有控件只创建一次）
 */
void create_weather_ui(void)
{
    // 1. 创建城市标签（固定显示“临沂”，也可读取basic的city变量）
    city_label = lv_label_create(lv_screen_active());
    lv_label_set_recolor(city_label, true);         // 允许文字变色
    lv_label_set_text(city_label, "#0000ff Linyi"); // 直接用char字符串，无需String
    lv_obj_set_style_text_font(city_label, &lv_font_montserrat_16, 0);
    lv_obj_set_style_text_align(city_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(city_label, LV_ALIGN_CENTER, -80, -80); // 左上位置

    // 2. 创建天气图标（图片控件，后续仅更新图片源）
    weather_img = lv_img_create(lv_screen_active());
    lv_obj_align(weather_img, LV_ALIGN_CENTER, 70, -80); // 右上位置
    // 初始化显示默认图标（避免空显示）
    lv_img_set_src(weather_img, &unknow);

    // 3. 创建星期/日期标签（后续更新内容）
    week_label = lv_label_create(lv_screen_active());
    lv_label_set_recolor(week_label, true);
    lv_obj_set_style_text_font(week_label, &lv_font_montserrat_16, 0);
    lv_obj_set_style_text_align(week_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(week_label, LV_ALIGN_CENTER, 0, 20); // 中下位置

    // 4. 创建时间标签（后续更新内容）
    time_label = lv_label_create(lv_screen_active());
    lv_label_set_recolor(time_label, true);
    lv_obj_set_style_text_font(time_label, &lv_font_montserrat_48, 0); // 大号字体
    lv_obj_set_style_text_align(time_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(time_label, LV_ALIGN_CENTER, 0, -20); // 中间位置

    // 5. 创建温湿度标签（后续更新内容）
    tem_hum_label = lv_label_create(lv_screen_active());
    lv_label_set_recolor(tem_hum_label, true);
    lv_obj_set_style_text_font(tem_hum_label, &lv_font_montserrat_16, 0);
    lv_obj_set_style_text_align(tem_hum_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(tem_hum_label, LV_ALIGN_CENTER, 0, 80); // 下位置

    // 初始化更新一次（确保启动时有内容）
    update_weather(weather_condition);     // 从basic读取天气（char数组）
    update_week(weekday);                  // 从basic读取星期（char数组）
    update_time(hour, minute);             // 从basic读取时间（int）
    update_tem_hum(temperature, humidity); // 从basic读取温湿度（int）
}

/**
 * 更新天气图标（根据basic的weather_condition（char数组）匹配图标）
 */
void update_weather(const char *weather_cond)
{
    // char字符串比较用strcmp，返回0表示相等（不能用==）
    if (strcmp(weather_cond, "晴") == 0)
        lv_img_set_src(weather_img, &Sunny);
    else if (strcmp(weather_cond, "多云") == 0)
        lv_img_set_src(weather_img, &Cloudy);
    else if (strcmp(weather_cond, "晴间多云") == 0)
        lv_img_set_src(weather_img, &Partly_Cloudy);
    else if (strcmp(weather_cond, "阴") == 0)
        lv_img_set_src(weather_img, &Overcast);
    else if (strcmp(weather_cond, "小雨") == 0)
        lv_img_set_src(weather_img, &Light_Rain);
    else if (strcmp(weather_cond, "中雨") == 0)
        lv_img_set_src(weather_img, &Moderate_Rain);
    else if (strcmp(weather_cond, "大雨") == 0)
        lv_img_set_src(weather_img, &Heavy_Rain);
    else if (strcmp(weather_cond, "暴雨") == 0)
        lv_img_set_src(weather_img, &Storm);
    else if (strcmp(weather_cond, "小雪") == 0)
        lv_img_set_src(weather_img, &Light_Snow);
    else if (strcmp(weather_cond, "大雪") == 0)
        lv_img_set_src(weather_img, &Heavy_Snow);
    else if (strcmp(weather_cond, "暴雪") == 0)
        lv_img_set_src(weather_img, &Snowstorm);
    else if (strcmp(weather_cond, "雨夹雪") == 0)
        lv_img_set_src(weather_img, &Sleet);
    else if (strcmp(weather_cond, "雾") == 0)
        lv_img_set_src(weather_img, &Foggy);
    else if (strcmp(weather_cond, "沙尘暴") == 0)
        lv_img_set_src(weather_img, &Duststorm);
    else if (strcmp(weather_cond, "霾") == 0)
        lv_img_set_src(weather_img, &Haze);
    else
        lv_img_set_src(weather_img, &unknow); // 未匹配时显示默认图标
}

/**
 * 更新星期/日期：汉字星期→英文简写，再拼接日期显示
 * 输入：const char* weekday（汉字星期，如“星期一”）
 */
void update_week(const char *weekday)
{
    // 1. 汉字星期 → 英文简写（核心转换逻辑）
    const char *week_en = "Unknown"; // 默认值：未知时显示
    if (strcmp(weekday, "星期一") == 0)
        week_en = "Mon.";
    else if (strcmp(weekday, "星期二") == 0)
        week_en = "Tue.";
    else if (strcmp(weekday, "星期三") == 0)
        week_en = "Wed.";
    else if (strcmp(weekday, "星期四") == 0)
        week_en = "Thu.";
    else if (strcmp(weekday, "星期五") == 0)
        week_en = "Fri.";
    else if (strcmp(weekday, "星期六") == 0)
        week_en = "Sat.";
    else if (strcmp(weekday, "星期日") == 0)
        week_en = "Sun.";

    // 2. 拼接“日期+英文星期”（格式：2025-10-09 Wed.）
    snprintf(week_buf, sizeof(week_buf), "#0000ff %04d-%02d-%02d %s",
             year, month, day, week_en); // 用转换后的week_en替代原汉字

    // 3. 更新标签显示（原有逻辑不变）
    lv_label_set_text(week_label, week_buf);
}

/**
 * 更新时间（拼接basic的hour/minute（int））
 */
void update_time(int hour, int minute)
{
    snprintf(time_buf, sizeof(time_buf), "#0000ff %02d:%02d",
             hour, minute, second); // 格式：19:45
    lv_label_set_text(time_label, time_buf);
}

/**
 * 更新温湿度（拼接basic的temperature/humidity（int））
 */
void update_tem_hum(int temp, int humi)
{
    snprintf(tem_hum_buf, sizeof(tem_hum_buf), "#0000ff %d°C / %d%%",
             temp, humi); // 格式：20°C / 89%
    lv_label_set_text(tem_hum_label, tem_hum_buf);
}