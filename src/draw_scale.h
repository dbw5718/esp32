#ifndef DRAW_SCALE_H
#define DRAW_SCALE_H

#include <lvgl.h>

//void create_scale();

void create_weather_ui();
void update_weather_ui(const char* city, const char* weather_condition, int tem, const char* winddirection);

#endif // THERMOMETER_H