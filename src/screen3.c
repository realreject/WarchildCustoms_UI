
#include "common_ui.h"
#include "globals.h"
#include "screen3.h"
#include "viking_bg_480_320.h"
#include "gauge/temperature_gauge_320_320.h"
#include "gauge/needle.h"

#define TITLE_FONT_SIZE &lv_font_montserrat_28
#define ICON_FONT_SIZE &lv_font_montserrat_48

// Define the tag for logging
// const char *TAG = "SCREEN3.C";

lv_obj_t *pointer_needle;
lv_timer_t *needle_timer;
int needle_index = 0;
int needle_direction = 1; // 1 for forward, -1 for reverse

// Define the total number of frames for smooth animation
#define TOTAL_FRAMES 60

// Define the start angle, stop angle, and total sweep
int start_angle = 0; // Adjust this value as needed
int stop_angle = 240; // Adjust this value as needed
int total_sweep;

void create_screen3()
{
    screen3 = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(screen3, lv_color_hex(0x050505), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_t *gauge_bg = lv_img_create(screen3);
    lv_img_set_src(gauge_bg, &temperature_gauge_320_320);
    lv_obj_align(gauge_bg, LV_ALIGN_CENTER, 0, 0);

    pointer_needle = lv_img_create(screen3);
    lv_img_set_src(pointer_needle, &needle_000);
    lv_obj_align(pointer_needle, LV_ALIGN_CENTER, 0, 0);
 
    lv_obj_t *label3 = lv_label_create(screen3);
    lv_label_set_text(label3, "METER");
    lv_obj_set_style_text_color(label3, lv_color_white(), 0);
    lv_obj_set_style_text_font(label3, TITLE_FONT_SIZE, 0);
    lv_obj_align(label3, LV_ALIGN_CENTER, 0, 135);

    create_home_button(screen3);

    // Calculate total_sweep
    total_sweep = stop_angle - start_angle;
}

void loop_needle(lv_timer_t *timer)
{
   // Calculate the rotation angle based on the current frame and the total sweep
   int angle = start_angle + (needle_index * total_sweep) / TOTAL_FRAMES;

   // Rotate the needle image by the calculated angle
   lv_img_set_angle(pointer_needle, angle * 10); // lv_img_set_angle uses 0.1 degree units

   // Increment or decrement the index and reverse direction if necessary
   needle_index += needle_direction;
   if (needle_index >= TOTAL_FRAMES || needle_index <= 0)
   {
       needle_direction *= -1; // Reverse direction
   }      
}

void set_needle_to_start()
{
    // Set the needle to the start angle
    lv_img_set_angle(pointer_needle, start_angle * 10); // lv_img_set_angle uses 0.1 degree units
}

void set_needle_to_stop()
{
    // Set the needle to the stop angle
    lv_img_set_angle(pointer_needle, stop_angle * 10); // lv_img_set_angle uses 0.1 degree units
}