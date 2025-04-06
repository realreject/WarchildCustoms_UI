
#include "common_ui.h"
#include "globals.h"
#include "screen3.h"
#include "gauge/temperature_gauge_320_320.h"
#include "gauge/needle.h"

// Define the tag for logging
// const char *TAG = "SCREEN3.C";

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

    create_title(screen3, "METER", 0, 135); // Create the title using the common function
    create_home_button(screen3);

    // Calculate total_sweep
    total_sweep = stop_angle - start_angle;
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