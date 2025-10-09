#ifndef DISPLAY_H
#define DISPLAY_H

#include <lvgl.h>
#include <TFT_eSPI.h>
#include "config.h"

extern TFT_eSPI tft;

void display_init();
void my_disp_flush(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map);
uint32_t my_tick(void);

#endif // DISPLAY_H