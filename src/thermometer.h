#ifndef THERMOMETER_H
#define THERMOMETER_H

#include <lvgl.h>

void create_thermometer();
void set_temperature(int temp, bool anim = true);

#endif // THERMOMETER_H
