#include "include.h"
#include "globals.h"


#define TITLE_FONT_SIZE &lv_font_montserrat_28
#define ICON_FONT_SIZE &lv_font_montserrat_48

void create_screen2()
{
    screen2 = lv_obj_create(NULL);
    lv_obj_t *bg_img2 = lv_img_create(screen2);
    lv_img_set_src(bg_img2, &viking_bg_480_320);
    lv_obj_align(bg_img2, LV_ALIGN_CENTER, 0, 0);

    lv_obj_t *label2 = lv_label_create(screen2);
    lv_label_set_text(label2, "FOOTWELL LAMPS");
    lv_obj_set_style_text_color(label2, lv_color_white(), 0);
    lv_obj_set_style_text_font(label2, TITLE_FONT_SIZE, 0);
    lv_obj_align(label2, LV_ALIGN_CENTER, 0, -135);

    create_color_wheel(screen2);

    lv_colorwheel_set_rgb(color_wheel, selected_color);

    power_btn = lv_label_create(screen2);
    lv_label_set_text(power_btn, LV_SYMBOL_POWER);
    lv_obj_set_style_text_font(power_btn, ICON_FONT_SIZE, 0);
    lv_obj_align(power_btn, LV_ALIGN_CENTER, 0, 0);

    lv_obj_add_flag(power_btn, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(power_btn, power_btn_event_handler, LV_EVENT_ALL, NULL);

    // Debug print to check power status
    ESP_LOGI("create_screen2", "Power status during screen creation: %d", power_status);

    if (power_status)
    {
        lv_obj_set_style_text_color(power_btn, selected_color, 0);
    }
    else
    {
        lv_obj_set_style_text_color(power_btn, lv_color_hex(0x3c3c3c), 0);
    }

    create_home_button(screen2);

    // create a slider bar lelow the color wheel
    brightness_slider = lv_slider_create(screen2);
    lv_obj_set_width(brightness_slider, 200);
    lv_obj_align(brightness_slider, LV_ALIGN_CENTER, 0, 140);
    lv_obj_add_event_cb(brightness_slider, brightness_slider_event_cb, LV_EVENT_VALUE_CHANGED, NULL);

    // Debug print to check brightness value during screen creation
    ESP_LOGI("create_screen2", "Initial brightness value: %d", glow_brightness);

    // set the initial slider value
    lv_slider_set_range(brightness_slider, 0, 255);
    lv_slider_set_value(brightness_slider, glow_brightness, LV_ANIM_OFF);

    // update the slider's color based on the selected color
    lv_obj_set_style_bg_color(brightness_slider, selected_color, LV_PART_INDICATOR);
    lv_obj_set_style_bg_color(brightness_slider, selected_color, LV_PART_KNOB);
}

// Function to create the color wheel and handle color changes
void create_color_wheel(lv_obj_t *parent)
{
      color_wheel = lv_colorwheel_create(parent, true);
      lv_obj_set_size(color_wheel, 200, 200);           // Set size of the color wheel
      lv_obj_align(color_wheel, LV_ALIGN_CENTER, 0, 0); // Align the color wheel in the center of the parent

      // Adjust the line width to make the arc/circle wider
      lv_obj_set_style_arc_width(color_wheel, 20, 0); // Set the arc width to 20 (adjust as needed)

      // Set the initial color of the knob to the saved color
      lv_obj_set_style_bg_color(color_wheel, selected_color, LV_PART_KNOB);

      // Add an event callback to handle color changes
      lv_obj_add_event_cb(color_wheel, color_wheel_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
}
