#include <lvgl.h>
#include <TFT_eSPI.h>
#include <Arduino.h>

/*Set to your screen resolution and rotation*/
#define TFT_HOR_RES   240
#define TFT_VER_RES   240
#define TFT_ROTATION  LV_DISPLAY_ROTATION_0

// 2. 引脚定义（必须与User_Setup.h完全一致！）
#define TFT_BL_PIN    21  // 背光引脚（对应User_Setup的TFT_BL）
#define TFT_DC_PIN    18  // 数据/命令引脚（对应User_Setup的TFT_DC）
#define TFT_CS_PIN    5   // 片选引脚（对应User_Setup的TFT_CS）
#define TFT_RST_PIN   4   // 复位引脚（对应User_Setup的TFT_RST）

/*LVGL draw into this buffer, 1/10 screen size usually works well. The size is in bytes*/
#define DRAW_BUF_SIZE (TFT_HOR_RES * TFT_VER_RES / 10 * (LV_COLOR_DEPTH / 8))

// 3. LVGL显示缓冲区（240x240屏幕，10行双缓冲，RGB565格式）
#define BUF_LINE_COUNT 10
static lv_color_t draw_buf[BUF_LINE_COUNT * TFT_HOR_RES];  // 仅保留这一行
TFT_eSPI tft = TFT_eSPI();  // 创建TFT对象

#if LV_USE_LOG != 0
void my_print( lv_log_level_t level, const char * buf )
{
    LV_UNUSED(level);
    Serial.println(buf);
    Serial.flush();
}
#endif

/* LVGL calls it when a rendered image needs to copied to the display*/
void my_disp_flush( lv_display_t *disp, const lv_area_t *area, uint8_t * px_map)
{
    tft.startWrite();  // 开始SPI通信
    // 设置屏幕显示区域（area是LVGL渲染的区域）
    tft.setAddrWindow(area->x1, area->y1, area->x2 - area->x1 + 1, area->y2 - area->y1 + 1);
    // 发送像素数据（px_map是LVGL的渲染缓存，转16位RGB565格式）
    tft.pushColors((uint16_t*)px_map, (area->x2 - area->x1 + 1) * (area->y2 - area->y1 + 1), true);
    tft.endWrite();    // 结束SPI通信
    lv_display_flush_ready(disp);
}

/*use Arduinos millis() as tick source*/
static uint32_t my_tick(void)
{
    return millis();
}

void setup()
{
    String LVGL_Arduino = "Hello Arduino! ";
    LVGL_Arduino += String('V') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();

    Serial.begin( 115200 );
    Serial.println( LVGL_Arduino );

    // 1. 初始化TFT屏幕
    tft.begin();                  // 初始化ST7789
    tft.setRotation(TFT_ROTATION); // 设置屏幕旋转
    tft.fillScreen(TFT_BLACK);    // 清屏（避免残留画面）

    // 2. 初始化背光（关键！确保与User_Setup的TFT_BACKLIGHT_ON一致）
    pinMode(TFT_BL_PIN, OUTPUT);
    digitalWrite(TFT_BL_PIN, TFT_BACKLIGHT_ON);  // 打开背光（HIGH/LOW与User_Setup匹配）

    lv_init();

    /*Set a tick source so that LVGL will know how much time elapsed. */
    lv_tick_set_cb(my_tick);

    lv_display_t * disp;

    /*Initialize the (dummy) input device driver*/
    lv_indev_t * indev = lv_indev_create();
    //lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER); /*Touchpad should have POINTER type*/
    //lv_indev_set_read_cb(indev, my_touchpad_read);

    /* Create a simple label
     * ---------------------
     lv_obj_t *label = lv_label_create( lv_screen_active() );
     lv_label_set_text( label, "Hello Arduino, I'm LVGL!" );
     lv_obj_align( label, LV_ALIGN_CENTER, 0, 0 );

     * Try an example. See all the examples
     *  - Online: https://docs.lvgl.io/master/examples.html
     *  - Source codes: https://github.com/lvgl/lvgl/tree/master/examples
     * ----------------------------------------------------------------

     lv_example_btn_1();

     * Or try out a demo. Don't forget to enable the demos in lv_conf.h. E.g. LV_USE_DEMO_WIDGETS
     * -------------------------------------------------------------------------------------------

     lv_demo_widgets();
     */

    lv_obj_t *label = lv_label_create( lv_screen_active() );
    lv_label_set_text( label, "Hello Arduino, I'm LVGL!" );
    lv_obj_align( label, LV_ALIGN_CENTER, 0, 0 );

    Serial.println( "Setup done" );
}

void loop()
{
    lv_timer_handler(); /* let the GUI do its work */
    delay(5); /* let this time pass */
}
