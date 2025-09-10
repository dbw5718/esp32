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

void createScale(){
   // 1. 创建环形温度计对象
lv_obj_t * thermometer = lv_scale_create (lv_screen_active ());
lv_obj_set_size (thermometer, 200, 200); // 温度计大小
lv_scale_set_label_show (thermometer, true); // 显示温度标签
lv_scale_set_mode (thermometer, LV_SCALE_MODE_ROUND_OUTER); // 环形外刻度模式
lv_obj_center (thermometer); // 屏幕居中
// 2. 基础参数配置（温度范围、刻度）
lv_scale_set_range (thermometer, -20, 40); // 温度范围：-20~40°C
lv_scale_set_total_tick_count (thermometer, 61); // 总刻度数（-20 到 40 共 61 个点）
lv_scale_set_major_tick_every (thermometer, 10); // 每 10°C 一个主刻度（带标签）
static const char * temp_labels[] = {"-20°C", "-10°C", "0°C", "10°C", "20°C", "30°C", "40°C", NULL};
lv_scale_set_text_src (thermometer, temp_labels); // 自定义标签
// 3. 默认样式（0~30°C，绿色区域）
// 3.1 主刻度样式（LV_PART_INDICATOR：主刻度线）
lv_obj_set_style_line_color (thermometer, lv_palette_darken (LV_PALETTE_GREEN, 2), LV_PART_INDICATOR);
lv_obj_set_style_line_width (thermometer, 2, LV_PART_INDICATOR); // 主刻度线宽
lv_obj_set_style_length (thermometer, 15, LV_PART_INDICATOR); // 主刻度长度
// 3.2 小刻度样式（LV_PART_ITEMS：小刻度线）
lv_obj_set_style_line_color (thermometer, lv_palette_lighten (LV_PALETTE_GREEN, 1), LV_PART_ITEMS);
lv_obj_set_style_line_width (thermometer, 1, LV_PART_ITEMS); // 小刻度线宽
lv_obj_set_style_length (thermometer, 8, LV_PART_ITEMS); // 小刻度长度
// 3.3 环形基线样式（LV_PART_MAIN：环形主体）
lv_obj_set_style_arc_color (thermometer, lv_palette_darken (LV_PALETTE_GREEN, 2), LV_PART_MAIN);
lv_obj_set_style_arc_width (thermometer, 3, LV_PART_MAIN); // 环形线宽
// 3.4 标签样式（无 LV_PART_LABEL，用 LV_PART_ANY 适配所有部件）
lv_obj_set_style_text_font (thermometer, LV_FONT_DEFAULT, LV_PART_ANY); // 标签字体
lv_obj_set_style_text_color (thermometer, lv_palette_darken (LV_PALETTE_GREEN, 3), LV_PART_ANY); // 标签颜色（默认绿色）
// 4. 低温区域（-20~0°C，蓝色区域）
static lv_style_t cold_indicator; // 低温主刻度样式
static lv_style_t cold_items; // 低温小刻度样式
static lv_style_t cold_main; // 低温环形基线样式
lv_style_init (&cold_indicator);
lv_style_set_line_color (&cold_indicator, lv_palette_darken (LV_PALETTE_BLUE, 2)); // 蓝色主刻度
lv_style_set_line_width (&cold_indicator, 2);
lv_style_set_length (&cold_indicator, 15);
lv_style_init (&cold_items);
lv_style_set_line_color (&cold_items, lv_palette_lighten (LV_PALETTE_BLUE, 1)); // 浅蓝色小刻度
lv_style_set_line_width (&cold_items, 1);
lv_style_set_length (&cold_items, 8);
lv_style_init (&cold_main);
lv_style_set_arc_color (&cold_main, lv_palette_darken (LV_PALETTE_BLUE, 2)); // 蓝色环形基线
lv_style_set_arc_width (&cold_main, 3);
// 添加低温分段并应用样式
lv_scale_section_t * cold_sec = lv_scale_add_section (thermometer);
lv_scale_set_section_range (thermometer, cold_sec, -20, 0); // 低温范围
lv_scale_set_section_style_indicator (thermometer, cold_sec, &cold_indicator);
lv_scale_set_section_style_items (thermometer, cold_sec, &cold_items);
lv_scale_set_section_style_main (thermometer, cold_sec, &cold_main);
// 5. 高温区域（30~40°C，红色区域）
static lv_style_t hot_indicator; // 高温主刻度样式
static lv_style_t hot_items; // 高温小刻度样式
static lv_style_t hot_main; // 高温环形基线样式
lv_style_init (&hot_indicator);
lv_style_set_line_color (&hot_indicator, lv_palette_darken (LV_PALETTE_RED, 2)); // 红色主刻度
lv_style_set_line_width (&hot_indicator, 2);
lv_style_set_length (&hot_indicator, 15);
lv_style_init (&hot_items);
lv_style_set_line_color (&hot_items, lv_palette_lighten (LV_PALETTE_RED, 1)); // 浅红色小刻度
lv_style_set_line_width (&hot_items, 1);
lv_style_set_length (&hot_items, 8);
lv_style_init (&hot_main);
lv_style_set_arc_color (&hot_main, lv_palette_darken (LV_PALETTE_RED, 2)); // 红色环形基线
lv_style_set_arc_width (&hot_main, 3);
// 添加高温分段并应用样式
lv_scale_section_t * hot_sec = lv_scale_add_section (thermometer);
lv_scale_set_section_range (thermometer, hot_sec, 30, 40); // 高温范围
lv_scale_set_section_style_indicator (thermometer, hot_sec, &hot_indicator);
lv_scale_set_section_style_items (thermometer, hot_sec, &hot_items);
lv_scale_set_section_style_main (thermometer, hot_sec, &hot_main);
// 6. 指针样式（LV_PART_KNOB：刻度指针）
static lv_style_t pointer_style;
lv_style_init (&pointer_style);
lv_style_set_line_color (&pointer_style, lv_palette_darken (LV_PALETTE_GREY, 3)); // 深灰色指针
lv_style_set_line_width (&pointer_style, 3); // 指针线宽
lv_style_set_length (&pointer_style, 20); // 指针长度
lv_obj_add_style (thermometer, &pointer_style, LV_PART_KNOB);
// 7. 设置初始温度（25°C，带动画过渡）
lv_bar_set_value (thermometer, 25, LV_ANIM_ON);
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
    // lv_obj_t *label = lv_label_create( lv_screen_active() );
    // lv_label_set_text( label, "Hello Arduino, I'm LVGL!" );
    // // 修复3：用默认启用的14号字体，避免字体未定义
    // lv_obj_set_style_text_font(label, &lv_font_montserrat_14, 0);
    // // 调试4：设置红色文本，对比黑色背景更易见
    // lv_obj_set_style_text_color(label, lv_color_hex(0xFF0000), 0);
    // lv_obj_align( label, LV_ALIGN_CENTER, 0, 0 );

    createScale(); // 创建环形温度计

    Serial.println( "Setup done" );
}

void loop()
{
    // 修复4：把delay(5000)改为5ms，确保LVGL能高频处理绘制
    lv_timer_handler(); /* let the GUI do its work */
    delay(5); /* 短延迟，不阻塞UI */
}