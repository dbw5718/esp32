#include <lvgl.h>
#include <TFT_eSPI.h>

// 1. 定义屏幕实际分辨率（必须与你的 ST7789 屏幕一致！）
#define SCREEN_WIDTH  240  // 例如 240x240 或 240x320 屏幕
#define SCREEN_HEIGHT 240

// 2. 初始化屏幕对象
TFT_eSPI tft = TFT_eSPI();

// 3. LVGL 显示缓冲区（用具体数值定义，确保编译时确定大小）
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[SCREEN_WIDTH * 10];  // 屏幕宽度 × 10 行

// 4. 屏幕刷新回调函数
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);

  tft.startWrite();
  tft.setAddrWindow(area->x1, area->y1, w, h);
  tft.pushColors(&color_p->full, w * h, true);
  tft.endWrite();

  lv_disp_flush_ready(disp);
}

// 5. 创建测试 UI
void create_ui() {
  lv_obj_t *label = lv_label_create(lv_scr_act());
  lv_label_set_text(label, "Hello LVGL!");
  lv_obj_align(label, LV_ALIGN_CENTER, 0, -50);

  lv_obj_t *btn = lv_btn_create(lv_scr_act());
  lv_obj_set_size(btn, 120, 50);
  lv_obj_align(btn, LV_ALIGN_CENTER, 0, 50);

  lv_obj_t *btn_label = lv_label_create(btn);
  lv_label_set_text(btn_label, "Click Me");
  lv_obj_center(btn_label);
}

void setup() {
  Serial.begin(115200);

  // 初始化背光
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);
  tft.init();
  tft.setRotation(0);

  // 初始化 LVGL
  lv_init();

  // 初始化缓冲区（用具体数值）
  lv_disp_draw_buf_init(&draw_buf, buf, NULL, SCREEN_WIDTH * 10);

  // 注册显示驱动
  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  disp_drv.hor_res = SCREEN_WIDTH;   // 使用定义的屏幕宽度
  disp_drv.ver_res = SCREEN_HEIGHT;  // 使用定义的屏幕高度
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register(&disp_drv);

  // 创建 UI
  create_ui();
}

void loop() {
  lv_timer_handler();
  delay(5);
}