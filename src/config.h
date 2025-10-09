#ifndef CONFIG_H
#define CONFIG_H

#include <lvgl.h>

/* 屏幕分辨率和旋转配置 */
#define TFT_HOR_RES   240
#define TFT_VER_RES   240
#define TFT_ROTATION_TFT  3                  // 给tft.setRotation的整数
#define TFT_ROTATION_LVGL LV_DISPLAY_ROTATION_270  // 给LVGL的枚举

/* 引脚定义 */
#define TFT_BL_PIN    21  
#define TFT_DC_PIN    18  
#define TFT_CS_PIN    5   
#define TFT_RST_PIN   4   

/* LVGL缓冲区配置（增大缓冲区避免刷新卡住）*/
#define BUF_LINE_COUNT 8  // 缓冲区行数（15行足够支撑刻度绘制）
static lv_color_t draw_buf[BUF_LINE_COUNT * TFT_HOR_RES];  

// 1. 将LVGL专属内存池从16KB增大到24KB或32KB
#define LVGL_MEM_POOL_SIZE (24 * 1024) // 尝试24KB
// #define LVGL_MEM_POOL_SIZE (32 * 1024) // 如果24KB还不行，再尝试32KB
static uint8_t lvgl_heap[LVGL_MEM_POOL_SIZE];

#endif // CONFIG_H