#include "draw.h"
#include <TFT_eSPI.h>
#include <SPI.h>
#include <basic.h>



TFT_eSPI tft;       //实例化
ESP32Time rtc(0);   //时间补偿

//动画帧的索引
static int frameIndex = 0;
//动画的宽度
#define animationWidth 40
//动画的高度
#define animationHeight 40
//局部刷新的参数，都是”像素/8“的值
int x = 11;
int y = 3;
int w = animationWidth / 8;
int h = animationHeight / 8;

//定义太空人动画显示所需要的帧延时变量
unsigned long previousTime = 0;  // 上一次执行任务的时间
unsigned long delayTime = 100;  // 延时时间（毫秒）
void astronaut_show(){
    unsigned long currentTime = millis();  // 获取当前时间
    if (currentTime - previousTime >= delayTime) 
        {
            tft.drawXBitmap(200, 200, frameData[frameIndex], 40, 40, TFT_BLACK, TFT_WHITE);
            frameIndex++;
            // tft.setAddrWindow(x, y, 40, 40);

            //播放到最后一帧之后，又回到第一帧开始播放
    if (frameIndex >= FRAME_NUM) 
    {
      frameIndex = 0;
    }

    previousTime = currentTime;  // 更新上一次执行任务的时间
  }
}

void time_show(int hour, int minute, int second) {
    tft.setTextColor(TFT_WHITE,TFT_BLACK); // Orange
    tft.setTextSize(2);

    char timeStr[9];
    sprintf(timeStr, "%02d:%02d:%02d", hour, minute, second);

    int textWidth = tft.textWidth(timeStr);
    int textHeight = 16; // Assuming text height is 16 pixels
    int x = (tft.width() - textWidth) / 2; // Center
    int y = (tft.height() - textHeight) / 2; // Center

    tft.fillRect(x, y, textWidth, textHeight, TFT_BLACK); // Clear the area
    tft.drawString(timeStr, x, y, 2); // Draw the time string
}

void weather_show(String city, String weather_condition, String winddirection, int temp) {
    tft.setTextColor(TFT_WHITE, TFT_BLACK);  // 设置文本颜色为白色，背景色为黑色
    tft.setTextSize(2);  // 设置文本大小

    // 城市名称
    tft.drawString(city, 0, 0);  // 在左上角显示城市名称

    // 天气状况
    int weatherX = tft.textWidth(city) + 5;  // 在城市名称后留一些空间
    tft.drawString(weather_condition, weatherX, 0);

    // 风向
    int windX = weatherX + tft.textWidth(weather_condition) + 5;  // 在天气状况后留一些空间
    tft.drawString(winddirection, windX, 0);

    // 温度
    String tempStr = String(temp) + "C";  // 将温度转换为字符串并添加摄氏度符号
    int tempX = windX + tft.textWidth(winddirection) + 5;  // 在风向后留一些空间
    tft.drawString(tempStr, tempX, 0);
}








