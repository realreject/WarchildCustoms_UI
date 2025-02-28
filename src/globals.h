#ifndef GLOBALS_H
#define GLOBALS_H


#include <lvgl.h>

//for screen creation
extern lv_obj_t *screen1, *screen2, *screen3;
extern lv_obj_t *color_wheel, *brightness_slider, *power_btn, *meter;
extern lv_obj_t *bg_img1, *bg_img2, *gauge_bg;       


//for ESP-NOW
extern int glow_red, glow_green, glow_blue, glow_power, glow_brightness;
extern bool power_status;
extern lv_color_t selected_color;


#endif // GLOBALS_H