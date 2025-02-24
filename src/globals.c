#include "globals.h"


lv_obj_t *screen1, *screen2, *screen3;
lv_obj_t *color_wheel, *brightness_slider, *power_btn, *meter;

lv_color_t selected_color;

int glow_red = 255;       // Default red value
int glow_green = 255;     // Default green value
int glow_blue = 255;      // Default blue value
int glow_power = 0;       // Default power state
int glow_brightness = 20; // Default brightness value
bool power_status = false;
lv_color_t selected_color;

lv_obj_t *meter, *rpm_label;
lv_meter_indicator_t *needle_indicator;