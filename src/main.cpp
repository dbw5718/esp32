#include <lvgl.h>
#include <TFT_eSPI.h>
#include <Arduino.h>
#include "display.h"
#include "draw_scale.h"  // 确保包含LVGL刻度组件头文件

// /* 屏幕分辨率和旋转（保持原有配置）*/
// #define TFT_HOR_RES   240
// #define TFT_VER_RES   240
// #define TFT_ROTATION_TFT  3                  // 给tft.setRotation的整数（3=270°）
// #define TFT_ROTATION_LVGL LV_DISPLAY_ROTATION_270  // 给LVGL的旋转枚举

// /* 引脚定义（保持原有）*/
// #define TFT_BL_PIN    21  
// #define TFT_DC_PIN    18  
// #define TFT_CS_PIN    5   
// #define TFT_RST_PIN   4   

// /* LVGL缓冲区配置（增大缓冲区避免刷新卡住）*/
// #define BUF_LINE_COUNT 15  // 缓冲区行数（15行足够支撑刻度绘制）
// static lv_color_t draw_buf[BUF_LINE_COUNT * TFT_HOR_RES];  
// static lv_draw_buf_t lv_draw_buf;  
// TFT_eSPI tft = TFT_eSPI();  

// /* LVGL刷新回调（保持原有，用于TFT绘制）*/
// void my_disp_flush(lv_display_t *disp, const lv_area_t *area, uint8_t * px_map)
// {
//     Serial.printf("LVGL刷新: x1=%d,y1=%d x2=%d,y2=%d\n", 
//                  area->x1, area->y1, area->x2, area->y2);

//     tft.startWrite();  
//     tft.setAddrWindow(area->x1, area->y1, area->x2 - area->x1 + 1, area->y2 - area->y1 + 1);
//     tft.pushColors((uint16_t*)px_map, (area->x2 - area->x1 + 1) * (area->y2 - area->y1 + 1), true);
//     tft.endWrite();    
//     lv_display_flush_ready(disp);  // 必须通知LVGL刷新完成
// }

// /* LVGL时钟源（保持原有）*/
// static uint32_t my_tick(void)
// {
//     return millis();
// }

// /* 创建刻度组件（严格基于lv_scale.c源码API）*/
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


// void setup()
// {
//     String LVGL_Arduino = "Hello Arduino! ";
//     LVGL_Arduino += String('V') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();

//     // 初始化串口（调试用）
//     Serial.begin(115200);
//     Serial.println(LVGL_Arduino);
//     delay(1000);  // 等待串口稳定

//     // 初始化TFT（硬件验证）
//     Serial.println("初始化TFT...");
//     tft.begin();
//     tft.setRotation(TFT_ROTATION_TFT);
//     tft.fillScreen(TFT_BLACK);
//     tft.drawPixel(120, 120, TFT_RED);  // 中心红点验证TFT正常
//     delay(1000);

//     // 打开背光
//     pinMode(TFT_BL_PIN, OUTPUT);
//     digitalWrite(TFT_BL_PIN, HIGH);

//     // 初始化LVGL
//     lv_init();
//     lv_tick_set_cb(my_tick);  // 设置时钟源

//     // 注册LVGL显示设备（关键步骤，不可漏）
//     Serial.println("注册LVGL显示设备...");
//     lv_draw_buf_init(&lv_draw_buf,
//                     TFT_HOR_RES, BUF_LINE_COUNT,
//                     LV_COLOR_FORMAT_RGB565,
//                     0,
//                     draw_buf,
//                     sizeof(draw_buf));
//     lv_display_t *disp = lv_display_create(TFT_HOR_RES, TFT_VER_RES);
//     lv_display_set_draw_buffers(disp, &lv_draw_buf, NULL);  // 绑定缓冲区
//     lv_display_set_flush_cb(disp, my_disp_flush);           // 绑定刷新回调
//     lv_display_set_rotation(disp, TFT_ROTATION_LVGL);       // 同步屏幕旋转

//     // 创建刻度组件
//     create_scale();
    
//     Serial.println("Setup done");
// }

void setup(){
    Serial.begin(115200);
    display_init();  // 初始化显示设备
    create_weather_ui();  // 创建天气UI布局

}

void loop()
{
    lv_timer_handler();  // LVGL主循环（必须高频调用）
    delay(5);            // 短延迟，不阻塞UI
}