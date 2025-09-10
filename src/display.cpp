#include "display.h"

TFT_eSPI tft = TFT_eSPI();
static lv_color_t draw_buf[BUF_LINE_COUNT * TFT_HOR_RES];  
static lv_draw_buf_t lv_draw_buf;

void display_init() {
    // 初始化TFT屏幕
    Serial.println("初始化TFT...");
    tft.begin();                  
    tft.setRotation(TFT_ROTATION_TFT);
    
    // 绘制测试图案，验证TFT硬件
    tft.fillScreen(TFT_BLACK);  
    tft.drawPixel(120, 120, TFT_RED);  // 屏幕中心画红色点
    delay(1000);
    
    // 初始化背光
    pinMode(TFT_BL_PIN, OUTPUT);
    digitalWrite(TFT_BL_PIN, HIGH);  // 打开背光
    
    // 初始化LVGL
    lv_init();
    
    // 设置LVGL时间源
    lv_tick_set_cb(my_tick);
    
    // 初始化LVGL缓冲区
    Serial.println("注册LVGL显示设备...");
    lv_draw_buf_init(&lv_draw_buf,
                    TFT_HOR_RES, BUF_LINE_COUNT,
                    LV_COLOR_FORMAT_RGB565,
                    0,
                    draw_buf,
                    sizeof(draw_buf));
    
    // 创建显示设备并绑定参数
    lv_display_t *disp = lv_display_create(TFT_HOR_RES, TFT_VER_RES);
    lv_display_set_draw_buffers(disp, &lv_draw_buf, NULL);
    lv_display_set_flush_cb(disp, my_disp_flush);
    lv_display_set_rotation(disp, TFT_ROTATION_LVGL);
}

void my_disp_flush(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map) {
    // 打印刷新区域，用于调试
    Serial.printf("LVGL刷新: x1=%d,y1=%d x2=%d,y2=%d\n", 
                 area->x1, area->y1, area->x2, area->y2);

    tft.startWrite();  
    tft.setAddrWindow(area->x1, area->y1, area->x2 - area->x1 + 1, area->y2 - area->y1 + 1);
    tft.pushColors((uint16_t*)px_map, (area->x2 - area->x1 + 1) * (area->y2 - area->y1 + 1), true);
    tft.endWrite();    
    lv_display_flush_ready(disp);
}

uint32_t my_tick(void) {
    return millis();
}
