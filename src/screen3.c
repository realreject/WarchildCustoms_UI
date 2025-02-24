#include "include.h"
#include "globals.h"
#include "screen3.h"

#define TITLE_FONT_SIZE &lv_font_montserrat_28
#define ICON_FONT_SIZE &lv_font_montserrat_48

// static lv_meter_indicator_t *needle_indicator;

void create_screen3()
{
    screen3 = lv_obj_create(NULL);
    lv_obj_t *bg_img3 = lv_img_create(screen3);
    lv_img_set_src(bg_img3, &viking_bg_480_320);
    lv_obj_align(bg_img3, LV_ALIGN_CENTER, 0, 0);

    lv_obj_t *label3 = lv_label_create(screen3);
    lv_label_set_text(label3, "METER");
    lv_obj_set_style_text_color(label3, lv_color_white(), 0);
    lv_obj_set_style_text_font(label3, TITLE_FONT_SIZE, 0);
    lv_obj_align(label3, LV_ALIGN_CENTER, 0, -135);

    create_home_button(screen3);
    create_meter();
}

// Create a meter on screen 3
void create_meter()
{
    // Define colors
    lv_color_t dark_blue = lv_color_make(10, 10, 44);
    lv_color_t custom_bg_color = lv_color_hex(0x7cd694);

    meter = lv_meter_create(screen3);
    lv_obj_center(meter);
    lv_obj_set_size(meter, 200, 200);


    // Set the background color of the meter
    lv_obj_set_style_bg_color(meter, custom_bg_color, LV_PART_MAIN);

    // Add a scale to the meter
    lv_meter_scale_t *scale = lv_meter_add_scale(meter);
    lv_meter_set_scale_range(meter, scale, 0, 200, 270, 90);
    lv_meter_set_scale_ticks(meter, scale, 21, 1, 5, dark_blue); 
    lv_meter_set_scale_major_ticks(meter, scale, 2, 2, 10, dark_blue, 15);

    // Add a needle to the scale
    needle_indicator = lv_meter_add_needle_line(meter, scale, 4, lv_palette_main(LV_PALETTE_RED), -10);

    // Create the RPM label and position it just below the center
    rpm_label = lv_label_create(meter);
    lv_label_set_text(rpm_label, "0 RPM"); // Initial text
    lv_obj_set_width(rpm_label, 100);                // Set a fixed width to accommodate larger text
    lv_obj_set_style_pad_all(rpm_label, 10, 0);      // Add padding around the label
    lv_obj_align(rpm_label, LV_ALIGN_CENTER, 10, 20); // Adjust the y-offset as needed
    lv_obj_set_style_text_color(rpm_label, dark_blue, 0); // Set the RPM label color to dark blue
}
