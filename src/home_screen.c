#include "home_screen.h"

#define ICON_FONT_SIZE &lv_font_montserrat_48

static const char *TAG = "home_screen.c";

void create_home_screen()
{
    home_screen = lv_obj_create(NULL);  
    initialize_global_bg(home_screen);        
    attach_bg_to_screen(home_screen); 
    create_title(home_screen, "MEGA CAB UI", 0, -135);
    create_bulb_button(home_screen);
    create_meter_button(home_screen);
    create_SDcard_button(home_screen);

    ESP_LOGE(TAG, "home_screen created successfully");
}

void create_bulb_button(lv_obj_t *parent)
{
      lv_obj_t *bulb_icon = lv_label_create(parent);            // Create a label for the icon
      lv_label_set_text(bulb_icon, LV_SYMBOL_EYE_OPEN);         // Set the text to the home symbol
      lv_obj_set_style_text_font(bulb_icon, ICON_FONT_SIZE, 0); // Set the font size to match the power icon
      lv_obj_align(bulb_icon, LV_ALIGN_CENTER, -180, -75);      // Center the icon with an offset

      // Make the icon interactive
      lv_obj_add_flag(bulb_icon, LV_OBJ_FLAG_CLICKABLE);   

      // Add styles to the icon
      lv_obj_set_style_text_color(bulb_icon, lv_color_white(), 0); // Set text color

      //touch call back
      lv_obj_add_event_cb(bulb_icon, go_to_led_controls_screen, LV_EVENT_CLICKED, NULL); // Add event callback
}

void create_meter_button(lv_obj_t *parent)
{
      lv_obj_t *meter_icon = lv_label_create(parent);            // Create a label for the icon
      lv_label_set_text(meter_icon, LV_SYMBOL_VIDEO);         // Set the text to the home symbol
      lv_obj_set_style_text_font(meter_icon, ICON_FONT_SIZE, 0); // Set the font size to match the power icon
      lv_obj_align(meter_icon, LV_ALIGN_CENTER, -120, -75);      // Center the icon with an offset

      // Make the icon interactive
      lv_obj_add_flag(meter_icon, LV_OBJ_FLAG_CLICKABLE);    

      // Add styles to the icon
      lv_obj_set_style_text_color(meter_icon, lv_color_white(), 0); // Set text color
      
      //touch call back
      lv_obj_add_event_cb(meter_icon, go_to_meter_screen, LV_EVENT_CLICKED, NULL); // Add event callback
}

void create_SDcard_button(lv_obj_t *parent){

      lv_obj_t *sdcard_icon = lv_label_create(parent);            // Create a label for the icon
      lv_label_set_text(sdcard_icon, LV_SYMBOL_SD_CARD);         // Set the text to the home symbol
      lv_obj_set_style_text_font(sdcard_icon, ICON_FONT_SIZE, 0); // Set the font size to match the power icon
      lv_obj_align(sdcard_icon, LV_ALIGN_CENTER, -60, -75);      // Center the icon with an offset
      
      // Make the icon interactive
      lv_obj_add_flag(sdcard_icon, LV_OBJ_FLAG_CLICKABLE);     
      
      // Add styles to the icon
      lv_obj_set_style_text_color(sdcard_icon, lv_color_white(), 0); // Set text color
            
       //touch call back
       lv_obj_add_event_cb(sdcard_icon, go_to_bg_sel_screen, LV_EVENT_CLICKED, NULL); // Add event callback
}