#ifndef GLOBALS_H
#define GLOBALS_H

#include "include.h"

//for screen creation
extern lv_obj_t *screen1, *screen2, *screen3;
extern lv_obj_t *color_wheel, *brightness_slider, *power_btn, *meter;


//for ESP-NOW
extern int glow_red, glow_green, glow_blue, glow_power, glow_brightness;
extern bool power_status;
extern lv_color_t selected_color;

//for meter
extern lv_obj_t *meter, *rpm_label;
extern lv_meter_indicator_t *needle_indicator;

#endif // GLOBALS_H