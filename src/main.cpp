// #include <lvgl.h>
// #include <TFT_eSPI.h>   // 如果你用 ILI9341/ST7789，建议用 TFT_eSPI

// TFT_eSPI tft = TFT_eSPI();

// #define DISP_BUF_SIZE (240 * 40) 

// static lv_disp_draw_buf_t draw_buf;
// static lv_color_t buf[DISP_BUF_SIZE];


// // 刷新回调：把 LVGL 的绘制结果推到屏幕
// void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
//     tft.startWrite();
//     tft.setAddrWindow(area->x1, area->y1, area->x2 - area->x1 + 1, area->y2 - area->y1 + 1);
//     tft.pushColors((uint16_t *)&color_p->full, (area->x2 - area->x1 + 1) * (area->y2 - area->y1 + 1), true);
//     tft.endWrite();
//     lv_disp_flush_ready(disp);
// }

// void setup() {
//     Serial.begin(115200);
//     Serial.println("LVGL minimal demo");

//     // 初始化 TFT
//     tft.begin();
//     tft.setRotation(1);

//     // 初始化 LVGL
//     lv_init();

//     lv_disp_draw_buf_init(&draw_buf, buf, NULL, DISP_BUF_SIZE);

//     static lv_disp_drv_t disp_drv;
//     lv_disp_drv_init(&disp_drv);
//     disp_drv.hor_res = 240;   // 改成你的屏幕分辨率
//     disp_drv.ver_res = 320;
//     disp_drv.flush_cb = my_disp_flush;
//     disp_drv.draw_buf = &draw_buf;
//     lv_disp_drv_register(&disp_drv);

//     // 创建一个简单标签
//     lv_obj_t *label = lv_label_create(lv_scr_act());
//     lv_label_set_text(label, "Hello LVGL!");
//     lv_obj_center(label);

//     // 创建一个简单的表盘
//     lv_obj_t * meter = lv_meter_create(lv_scr_act());
//     lv_obj_set_pos(meter, 100, 200);
//     // 设置表盘大小
//     lv_meter_scale_t * scale=lv_meter_add_scale(meter);
//     lv_meter_set_scale_ticks(meter, scale, 41, 2, 10, lv_palette_main(LV_PALETTE_GREY));
//     lv_meter_set_scale_major_ticks(meter, scale, 8, 4, 15, lv_color_black(), 10);
//     // 添加指针
//     lv_meter_indicator_t * indic = lv_meter_add_needle_line(meter, scale, 4, lv_palette_main(LV_PALETTE_RED), -10);
//     // 设置表盘范围和初始值
//     lv_meter_set_scale_range(meter, scale, 0, 100, 270, 90);
//     lv_meter_set_indicator_value(meter, indic, 50);


// }

// void loop() {
//     lv_timer_handler();  // 让 LVGL 刷新
//     delay(5);
// }
#include <TFT_eSPI.h>
#include "font_yahei24.h">

TFT_eSPI tft = TFT_eSPI();

void setup() {
  Serial.begin(115200);
  tft.begin();         // 初始化屏幕
  tft.loadFont(font_yahei24); // 加载中文字体
  tft.drawString("你好，世界！", 0, 0);
  tft.drawString("Hello World", 0, 30);
  tft.drawString("0123456789", 0, 60);
  tft.unloadFont();



void loop() {}