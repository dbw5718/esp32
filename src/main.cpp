#include <SPI.h>
#include <TFT_eSPI.h>
#include <lvgl.h>
#include "basic.h"

#define LV_HOR_RES_MAX 240 // Set this to your display's horizontal resolution


TFT_eSPI tft = TFT_eSPI(); // Create TFT instance
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[LV_HOR_RES_MAX * 10];


void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, area->x2 - area->x1 + 1, area->y2 - area->y1 + 1);
    tft.pushColors(&color_p->full, (area->x2 - area->x1 + 1) * (area->y2 - area->y1 + 1), true);
    tft.endWrite();
    lv_disp_flush_ready(disp);
}
  void setup()  
  {

    led_init();
    tft.init(); 
    tft.fillScreen(TFT_WHITE);
    tft.setRotation(1);
    tft.setTextColor(TFT_RED);
    tft.setTextSize(1);
    tft.setCursor(0,0,2);
    tft.println("Hello, LVGL!");

    
  }
  
  void loop()
  {
    
    delay(5);

  }
  

  
  


