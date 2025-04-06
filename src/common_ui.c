#include "common_ui.h"
#include "viking_bg_480_320.h"


// Defines
#define TITLE_FONT_SIZE &lv_font_montserrat_28
#define ICON_FONT_SIZE &lv_font_montserrat_48
#define CUSTOM_FONT &diner_font_set

// Function to create the home button 
void create_home_button(lv_obj_t *parent)
{
    lv_obj_t *home_icon = lv_label_create(parent);            // Create a label for the icon
    lv_label_set_text(home_icon, LV_SYMBOL_HOME);             // Set the text to the home symbol
    lv_obj_set_style_text_font(home_icon, ICON_FONT_SIZE, 0); // Set the font size to match the power icon
    lv_obj_align(home_icon, LV_ALIGN_CENTER, -185, 120);      // Center the icon with an offset

    // Make the icon interactive
    lv_obj_add_flag(home_icon, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(home_icon, go_to_home_screen, LV_EVENT_CLICKED, NULL); // Add event callback

    // Add styles to the icon
    lv_obj_set_style_text_color(home_icon, lv_color_white(), 0); // Set text color
}

void create_background(lv_obj_t *parent) {
    lv_obj_t *background = lv_img_create(parent);
    lv_img_set_src(background, &viking_bg_480_320); // Use the image directly from the C file
    lv_obj_align(background, LV_ALIGN_CENTER, 0, 0); // Center alignment
}

 void    create_title(lv_obj_t *parent, const char *text, uint16_t x, uint16_t y) {
    lv_obj_t *label = lv_label_create(parent);
    lv_label_set_text(label, text);
    lv_obj_set_style_text_color(label, lv_color_white(), 0);
    lv_obj_set_style_text_font(label, CUSTOM_FONT, 0);
    lv_obj_align(label, LV_ALIGN_CENTER, x, y); // Use x and y directly
    
} 

