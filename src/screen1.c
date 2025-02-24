#include "include.h"
#include "globals.h"

#define TITLE_FONT_SIZE &lv_font_montserrat_28
#define ICON_FONT_SIZE &lv_font_montserrat_48

void create_screen1()
{
    screen1 = lv_obj_create(NULL);
    lv_obj_t *bg_img1 = lv_img_create(screen1);
    lv_img_set_src(bg_img1, &viking_bg_480_320);
    lv_obj_align(bg_img1, LV_ALIGN_CENTER, 0, 0);

    lv_obj_t *label1 = lv_label_create(screen1);
    lv_label_set_text(label1, "MEGA CAB UI");
    lv_obj_set_style_text_color(label1, lv_color_white(), 0);
    lv_obj_set_style_text_font(label1, TITLE_FONT_SIZE, 0);
    lv_obj_align(label1, LV_ALIGN_CENTER, 0, -135);

    create_bulb_button(screen1);
    create_meter_button(screen1);
}

void create_bulb_button(lv_obj_t *parent)
{
      lv_obj_t *bulb_icon = lv_label_create(parent);            // Create a label for the icon
      lv_label_set_text(bulb_icon, LV_SYMBOL_EYE_OPEN);         // Set the text to the home symbol
      lv_obj_set_style_text_font(bulb_icon, ICON_FONT_SIZE, 0); // Set the font size to match the power icon
      lv_obj_align(bulb_icon, LV_ALIGN_CENTER, -180, -75);      // Center the icon with an offset

      // Make the icon interactive
      lv_obj_add_flag(bulb_icon, LV_OBJ_FLAG_CLICKABLE);
      lv_obj_add_event_cb(bulb_icon, go_to_screen1, LV_EVENT_CLICKED, NULL); // Add event callback

      // Add styles to the icon
      lv_obj_set_style_text_color(bulb_icon, lv_color_white(), 0); // Set text color

      lv_obj_add_event_cb(bulb_icon, go_to_screen2, LV_EVENT_CLICKED, NULL); // Add event callback
}

void create_meter_button(lv_obj_t *parent)
{
      lv_obj_t *meter_icon = lv_label_create(parent);            // Create a label for the icon
      lv_label_set_text(meter_icon, LV_SYMBOL_VIDEO);         // Set the text to the home symbol
      lv_obj_set_style_text_font(meter_icon, ICON_FONT_SIZE, 0); // Set the font size to match the power icon
      lv_obj_align(meter_icon, LV_ALIGN_CENTER, -120, -75);      // Center the icon with an offset

      // Make the icon interactive
      lv_obj_add_flag(meter_icon, LV_OBJ_FLAG_CLICKABLE);
      lv_obj_add_event_cb(meter_icon, go_to_screen1, LV_EVENT_CLICKED, NULL); // Add event callback

      // Add styles to the icon
      lv_obj_set_style_text_color(meter_icon, lv_color_white(), 0); // Set text color

      lv_obj_add_event_cb(meter_icon, go_to_screen3, LV_EVENT_CLICKED, NULL); // Add event callback
}