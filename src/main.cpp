#include <lvgl.h>
#include <TFT_eSPI.h>
#include <Arduino.h>

/* 屏幕分辨率和旋转（保持原有配置）*/
#define TFT_HOR_RES   240
#define TFT_VER_RES   240
#define TFT_ROTATION_TFT  3                  // 给tft.setRotation的整数（3=270°）
#define TFT_ROTATION_LVGL LV_DISPLAY_ROTATION_270  // 给LVGL的旋转枚举

/* 引脚定义（保持原有）*/
#define TFT_BL_PIN    21  
#define TFT_DC_PIN    18  
#define TFT_CS_PIN    5   
#define TFT_RST_PIN   4   

/* LVGL缓冲区配置（增大缓冲区避免刷新卡住）*/
#define BUF_LINE_COUNT 15  // 缓冲区行数（15行足够支撑刻度绘制）
static lv_color_t draw_buf[BUF_LINE_COUNT * TFT_HOR_RES];  
static lv_draw_buf_t lv_draw_buf;  
TFT_eSPI tft = TFT_eSPI();  

/* LVGL刷新回调（保持原有，用于TFT绘制）*/
void my_disp_flush(lv_display_t *disp, const lv_area_t *area, uint8_t * px_map)
{
    Serial.printf("LVGL刷新: x1=%d,y1=%d x2=%d,y2=%d\n", 
                 area->x1, area->y1, area->x2, area->y2);

    tft.startWrite();  
    tft.setAddrWindow(area->x1, area->y1, area->x2 - area->x1 + 1, area->y2 - area->y1 + 1);
    tft.pushColors((uint16_t*)px_map, (area->x2 - area->x1 + 1) * (area->y2 - area->y1 + 1), true);
    tft.endWrite();    
    lv_display_flush_ready(disp);  // 必须通知LVGL刷新完成
}

/* LVGL时钟源（保持原有）*/
static uint32_t my_tick(void)
{
    return millis();
}

/* 创建刻度组件（严格基于lv_scale.c源码API）*/
void create_scale() {
    Serial.println("创建Scale组件...");
    
    // 1. 创建刻度对象（源码中`lv_scale_create`存在）
    lv_obj_t *scale = lv_scale_create(lv_screen_active());
    if(!scale) {
        Serial.println("创建Scale失败!");
        return;
    }

    // 2. 核心刻度配置（全部使用源码存在的函数）
    lv_scale_set_mode(scale, LV_SCALE_MODE_HORIZONTAL_BOTTOM);  // 水平模式（刻度在下方）
    lv_scale_set_range(scale, 0, 100);                          // 数值范围：0-100
    lv_scale_set_total_tick_count(scale, 21);                   // 总刻度数：21个（0-100每5一个刻度）
    lv_scale_set_major_tick_every(scale, 5);                    // 主刻度间隔：每5个刻度1个主刻度（显示标签）
    lv_scale_set_label_show(scale, true);                       // 显示主刻度数值标签

    // 3. 刻度样式配置（对应源码中`scale_draw_indicator`的样式获取逻辑）
    // 3.1 主刻度样式（LV_PART_INDICATOR：源码中主刻度的PART）
    lv_obj_set_style_length(scale, 12, LV_PART_INDICATOR);      // 主刻度长度：12px
    lv_obj_set_style_line_width(scale, 3, LV_PART_INDICATOR);   // 主刻度线宽：3px
    lv_obj_set_style_line_color(scale, lv_color_hex(0xFF0000), LV_PART_INDICATOR);  // 主刻度颜色：红色
    // 3.2 次刻度样式（LV_PART_ITEMS：源码中次刻度的PART）
    lv_obj_set_style_length(scale, 6, LV_PART_ITEMS);           // 次刻度长度：6px
    lv_obj_set_style_line_width(scale, 1, LV_PART_ITEMS);       // 次刻度线宽：1px
    lv_obj_set_style_line_color(scale, lv_color_hex(0xFFFFFF), LV_PART_ITEMS);  // 次刻度颜色：白色
    // 3.3 刻度条背景样式（LV_PART_MAIN：源码中主体PART）
    lv_obj_set_style_bg_color(scale, lv_color_hex(0x333333), LV_PART_MAIN);      // 背景色：深灰
    lv_obj_set_style_border_color(scale, lv_color_hex(0xFFFFFF), LV_PART_MAIN);  // 边框色：白色
    lv_obj_set_style_border_width(scale, 2, LV_PART_MAIN);      // 边框宽：2px
    lv_obj_set_style_radius(scale, 4, LV_PART_MAIN);            // 圆角：4px

    // 4. 创建设置线指针（源码唯一支持的指示器：`lv_scale_set_line_needle_value`）
    lv_obj_t *needle = lv_line_create(scale);                  // 创建线对象作为指针
    lv_obj_set_style_line_width(needle, 3, LV_PART_MAIN);      // 指针线宽：3px
    lv_obj_set_style_line_color(needle, lv_color_hex(0x00FF00), LV_PART_MAIN);  // 指针颜色：绿色
    // 关联指针到刻度值（设置指针指向75的位置）
    lv_scale_set_line_needle_value(scale, needle, 20, 75);     // 20=指针长度，75=目标数值

    // 5. 刻度组件位置和大小（适配240x240屏幕）
    lv_obj_set_size(scale, 200, 40);  // 刻度条大小：宽200px，高40px
    lv_obj_align(scale, LV_ALIGN_CENTER, 0, 0);  // 屏幕居中显示

    Serial.println("Scale组件创建完成");
}

void setup()
{
    String LVGL_Arduino = "Hello Arduino! ";
    LVGL_Arduino += String('V') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();

    // 初始化串口（调试用）
    Serial.begin(115200);
    Serial.println(LVGL_Arduino);
    delay(1000);  // 等待串口稳定

    // 初始化TFT（硬件验证）
    Serial.println("初始化TFT...");
    tft.begin();
    tft.setRotation(TFT_ROTATION_TFT);
    tft.fillScreen(TFT_BLACK);
    tft.drawPixel(120, 120, TFT_RED);  // 中心红点验证TFT正常
    delay(1000);

    // 打开背光
    pinMode(TFT_BL_PIN, OUTPUT);
    digitalWrite(TFT_BL_PIN, HIGH);

    // 初始化LVGL
    lv_init();
    lv_tick_set_cb(my_tick);  // 设置时钟源

    // 注册LVGL显示设备（关键步骤，不可漏）
    Serial.println("注册LVGL显示设备...");
    lv_draw_buf_init(&lv_draw_buf,
                    TFT_HOR_RES, BUF_LINE_COUNT,
                    LV_COLOR_FORMAT_RGB565,
                    0,
                    draw_buf,
                    sizeof(draw_buf));
    lv_display_t *disp = lv_display_create(TFT_HOR_RES, TFT_VER_RES);
    lv_display_set_draw_buffers(disp, &lv_draw_buf, NULL);  // 绑定缓冲区
    lv_display_set_flush_cb(disp, my_disp_flush);           // 绑定刷新回调
    lv_display_set_rotation(disp, TFT_ROTATION_LVGL);       // 同步屏幕旋转

    // 创建刻度组件
    create_scale();

    Serial.println("Setup done");
}

void loop()
{
    lv_timer_handler();  // LVGL主循环（必须高频调用）
    delay(5);            // 短延迟，不阻塞UI
}