#include <lvgl.h>
#include <TFT_eSPI.h>   // 如果你用 ILI9341/ST7789，建议用 TFT_eSPI

TFT_eSPI tft = TFT_eSPI();

#define DISP_BUF_SIZE (240 * 40) 

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[DISP_BUF_SIZE];


// 刷新回调：把 LVGL 的绘制结果推到屏幕
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, area->x2 - area->x1 + 1, area->y2 - area->y1 + 1);
    tft.pushColors((uint16_t *)&color_p->full, (area->x2 - area->x1 + 1) * (area->y2 - area->y1 + 1), true);
    tft.endWrite();
    lv_disp_flush_ready(disp);
}

void setup() {
    Serial.begin(115200);
    Serial.println("LVGL minimal demo");

    // 初始化 TFT
    tft.begin();
    tft.setRotation(1);

    // 初始化 LVGL
    lv_init();

    lv_disp_draw_buf_init(&draw_buf, buf, NULL, DISP_BUF_SIZE);

    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = 240;   // 改成你的屏幕分辨率
    disp_drv.ver_res = 320;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);

    // 创建一个简单标签
    lv_obj_t *label = lv_label_create(lv_scr_act());
    lv_label_set_text(label, "Hello LVGL!");
    lv_obj_center(label);
}

void loop() {
    lv_timer_handler();  // 让 LVGL 刷新
    delay(5);
}
