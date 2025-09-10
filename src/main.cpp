#include <Arduino.h>
#include <lvgl.h>
#include "config.h"
#include "display.h"
#include "thermometer.h"

void setup() {
    String LVGL_Arduino = "Hello Arduino! ";
    LVGL_Arduino += String('V') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();

    Serial.begin(115200);
    Serial.println(LVGL_Arduino);
    delay(1000);  // 等待串口稳定

    // 初始化显示
    display_init();

    // 创建温度计
    create_thermometer();

    Serial.println("Setup done");
}

void loop() {
    lv_timer_handler(); /* 让GUI处理任务 */
    delay(5); /* 短延迟，不阻塞UI */
}
