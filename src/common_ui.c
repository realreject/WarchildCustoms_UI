#include "common_ui.h"
#include "event_handler.h"

// Defines
#define ICON_FONT_SIZE &lv_font_montserrat_48

// Function to create the home button
void create_home_button(lv_obj_t *parent)
{
    lv_obj_t *home_icon = lv_label_create(parent);            // Create a label for the icon
    lv_label_set_text(home_icon, LV_SYMBOL_HOME);             // Set the text to the home symbol
    lv_obj_set_style_text_font(home_icon, ICON_FONT_SIZE, 0); // Set the font size to match the power icon
    lv_obj_align(home_icon, LV_ALIGN_CENTER, -180, 120);      // Center the icon with an offset

    // Make the icon interactive
    lv_obj_add_flag(home_icon, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(home_icon, go_to_screen1, LV_EVENT_CLICKED, NULL); // Add event callback

    // Add styles to the icon
    lv_obj_set_style_text_color(home_icon, lv_color_white(), 0); // Set text color
}