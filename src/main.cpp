#include <lvgl.h>
#include <TFT_eSPI.h>
#include <stdlib.h>
#include <time.h>

#include <TFT_eSPI.h>

// 初始化TFT对象
TFT_eSPI tft = TFT_eSPI();

// 定义背光控制引脚（根据实际接线修改）
#define BACKLIGHT_PIN 2

void setup() {
  // 初始化串口
  Serial.begin(115200);
  
  // 配置背光引脚并开启背光
  pinMode(BACKLIGHT_PIN, OUTPUT);
  digitalWrite(BACKLIGHT_PIN, HIGH);
  
  // 初始化屏幕
  tft.init();
  
  // 设置屏幕旋转方向（0-3可选，根据实际显示调整）
  tft.setRotation(0);
  
  // 清屏为黑色
  tft.fillScreen(TFT_BLACK);
  
  // 设置文本颜色为白色
  tft.setTextColor(TFT_WHITE);
  
  // 设置文本大小（1-7，数字越大文字越大）
  tft.setTextSize(2);
  
  // 在指定位置打印文本（x=20, y=100）
  tft.setCursor(20, 100);
  tft.print("Hello World!");
  
  // 可以再打印一行不同颜色的文本
  tft.setTextColor(TFT_GREEN);
  tft.setTextSize(1);
  tft.setCursor(40, 130);
  tft.print("ESP32 + ST7789");
}

void loop() {
  // 不需要循环执行任何操作
  delay(1000);
}

    