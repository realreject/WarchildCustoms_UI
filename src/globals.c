#include "globals.h"

//for sreensaver
int current_brightness_level; // Tracks current brightness
bool screensaver_active; // Flag to indicate if dimming mode is active

//LVGL objects
lv_obj_t *home_screen, *led_controls_screen, *meter_screen, *bg_sel_screen;
lv_obj_t *color_wheel, *brightness_slider, *power_btn, *meter;
//lv_obj_t *background = NULL;

//testing global background object
lv_obj_t *global_bg_img = NULL;

//Color picker (footwell lighting)
lv_color_t selected_color;
int glow_red = 255;       // Default red value
int glow_green = 255;     // Default green value
int glow_blue = 255;      // Default blue value
int glow_power = 0;       // Default power state
int glow_brightness = 20; // Default brightness value
bool power_status = false;
lv_color_t selected_color;

//for meter
lv_obj_t *pointer_needle = NULL; 
int start_angle = 0;               
int stop_angle = 240;              
int total_sweep;

//custom fonts
lv_style_t db_font_style;

//image window
lv_obj_t *image_window = NULL;








