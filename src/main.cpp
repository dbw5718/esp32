#include <lvgl.h>
#include <TFT_eSPI.h>


// 1. 定义屏幕实际分辨率（必须与你的 ST7789 屏幕一致！）
#define SCREEN_WIDTH  240  // 例如 240x240 或 240x320 屏幕
#define SCREEN_HEIGHT 240

// 2. 初始化屏幕对象
TFT_eSPI tft = TFT_eSPI();

void setup() {
  Serial.begin(115200);

  // 初始化背光
  tft.init();
  tft.setRotation(2);
  tft.fillScreen(TFT_WHITE);
  tft.setTextColor(TFT_RED);
  tft.print("Hello World");

 
}

void loop() {
  
  delay(5);
}