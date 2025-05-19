#ifndef GLOBALS_H
#define GLOBALS_H

#include <lvgl.h>

//for screensaver
extern int current_brightness_level; // Tracks current brightness
extern bool screensaver_active; // Flag to indicate if dimming mode is active

//for screen creation
extern lv_obj_t *home_screen, *led_controls_screen, *meter_screen, *bg_sel_screen;
extern lv_obj_t *color_wheel, *brightness_slider, *power_btn, *meter;    
//extern lv_obj_t *background;

//testing global background object
extern lv_obj_t *global_bg_img;


//for ESP-NOW
extern int glow_red, glow_green, glow_blue, glow_power, glow_brightness;
extern bool power_status;
extern lv_color_t selected_color;

//for meter
#define TOTAL_FRAMES 100 // Adjust this value as needed
#define ANIMATION_DURATION 1000 // Adjust this value as needed
extern lv_obj_t *pointer_needle;
extern int start_angle;
extern int stop_angle;
extern int total_sweep;

//custom fonts
extern lv_style_t db_font_style;

//image window
extern lv_obj_t *image_window;

#endif // GLOBALS_H