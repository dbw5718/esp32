#include <lvgl.h>
#include <TFT_eSPI.h>
#include <Arduino.h>

/*Set to your screen resolution and rotation*/
#define TFT_HOR_RES   240
#define TFT_VER_RES   240
// 修复1：LVGL旋转用枚举（原整数3对应枚举LV_DISPLAY_ROTATION_270）
#define TFT_ROTATION_TFT  3                  // 给tft.setRotation的整数
#define TFT_ROTATION_LVGL LV_DISPLAY_ROTATION_270  // 给LVGL的枚举

// 2. 引脚定义（保持你原有的）
#define TFT_BL_PIN    21  
#define TFT_DC_PIN    18  
#define TFT_CS_PIN    5   
#define TFT_RST_PIN   4   

/*LVGL draw into this buffer, 1/10 screen size usually works well*/
#define DRAW_BUF_SIZE (TFT_HOR_RES * TFT_VER_RES / 10 * (LV_COLOR_DEPTH / 8))

// 3. LVGL显示缓冲区（保持你原有的，仅加LVGL缓冲区对象）
#define BUF_LINE_COUNT 10
static lv_color_t draw_buf[BUF_LINE_COUNT * TFT_HOR_RES];  
static lv_draw_buf_t lv_draw_buf;  // 新增：LVGL 9.3必须的缓冲区对象
TFT_eSPI tft = TFT_eSPI();  

/* LVGL刷新回调（加调试日志，确认是否在绘制）*/
void my_disp_flush( lv_display_t *disp, const lv_area_t *area, uint8_t * px_map)
{
    // 调试1：打印刷新区域，确认LVGL是否在工作
    Serial.printf("LVGL刷新: x1=%d,y1=%d x2=%d,y2=%d\n", 
                 area->x1, area->y1, area->x2, area->y2);

    tft.startWrite();  
    tft.setAddrWindow(area->x1, area->y1, area->x2 - area->x1 + 1, area->y2 - area->y1 + 1);
    tft.pushColors((uint16_t*)px_map, (area->x2 - area->x1 + 1) * (area->y2 - area->y1 + 1), true);
    tft.endWrite();    
    lv_display_flush_ready(disp);  // 必须保留，否则LVGL会停止绘制
}

/*use Arduinos millis() as tick source（保持你原有的）*/
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
    delay(1000);  // 调试2：等待串口稳定，避免日志丢失

    // 1. 初始化TFT屏幕（加调试，确认TFT是否正常）
    Serial.println("初始化TFT...");
    tft.begin();                  
    tft.setRotation(TFT_ROTATION_TFT); // 用你原有的旋转值3（整数）
    // 调试3：绘制黑色清屏+红色点，验证TFT硬件
    tft.fillScreen(TFT_BLACK);  
    tft.drawPixel(120, 120, TFT_RED);  // 屏幕中心画红色点（可视化验证）
    delay(1000);  // 显示1秒，确认TFT能亮

    //2. 初始化背光（保持你原有的）
    pinMode(TFT_BL_PIN, OUTPUT);
    digitalWrite(TFT_BL_PIN, HIGH);  // 打开背光


    lv_init();

    // /*Set a tick source（保持你原有的）*/
    lv_tick_set_cb(my_tick);

    // 修复2：新增LVGL显示设备注册（你之前漏了这步，导致绘制不输出到屏幕）
    Serial.println("注册LVGL显示设备...");
    // 2.1 初始化LVGL缓冲区
    lv_draw_buf_init(&lv_draw_buf,
                    TFT_HOR_RES, BUF_LINE_COUNT,
                    LV_COLOR_FORMAT_RGB565,
                    0,
                    draw_buf,
                    sizeof(draw_buf));
    // 2.2 创建显示设备并绑定参数
    lv_display_t *disp = lv_display_create(TFT_HOR_RES, TFT_VER_RES);
    lv_display_set_draw_buffers(disp, &lv_draw_buf, NULL);  // 绑定缓冲区
    lv_display_set_flush_cb(disp, my_disp_flush);           // 绑定刷新回调
    lv_display_set_rotation(disp, TFT_ROTATION_LVGL);       // 修复旋转类型错误

    // /* Create a simple label（保持你原有的，仅改字体为默认14号）*/
    Serial.println("创建Label...");
    lv_obj_t *label = lv_label_create( lv_screen_active() );
    lv_label_set_text( label, "Hello Arduino, I'm LVGL!" );
    // 修复3：用默认启用的14号字体，避免字体未定义
    lv_obj_set_style_text_font(label, &lv_font_montserrat_14, 0);
    // 调试4：设置红色文本，对比黑色背景更易见
    lv_obj_set_style_text_color(label, lv_color_hex(0xFF0000), 0);
    lv_obj_align( label, LV_ALIGN_CENTER, 0, 0 );

    Serial.println( "Setup done" );
}

void loop()
{
    // 修复4：把delay(5000)改为5ms，确保LVGL能高频处理绘制
    lv_timer_handler(); /* let the GUI do its work */
    delay(5); /* 短延迟，不阻塞UI */
}