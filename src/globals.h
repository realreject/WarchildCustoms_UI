#ifndef GLOBALS_H
#define GLOBALS_H

#include "include.h"

extern lv_obj_t *screen1, *screen2, *color_wheel, *brightness_slider, *power_btn;
extern lv_color_t selected_color;

extern int glow_red, glow_green, glow_blue, glow_power, glow_brightness;
extern bool power_status;


#endif // GLOBALS_H