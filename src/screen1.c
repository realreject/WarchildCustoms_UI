#include "screen1.h"


#define ICON_FONT_SIZE &lv_font_montserrat_48

void create_screen1()
{
    screen1 = lv_obj_create(NULL);
    create_background(screen1);

    //lv_obj_t *label = lv_label_create(screen1);
    create_title(screen1, "MEGA CAB UI", 0, -135); // Create the title using the common function/*   
    create_bulb_button(screen1);
    create_meter_button(screen1);
    create_SDcard_button(screen1);
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

      // Add styles to the icon
      lv_obj_set_style_text_color(meter_icon, lv_color_white(), 0); // Set text color
      
      //touch call back
      lv_obj_add_event_cb(meter_icon, go_to_screen3, LV_EVENT_CLICKED, NULL); // Add event callback
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
      lv_obj_add_event_cb(sdcard_icon, read_and_display_images, LV_EVENT_CLICKED, NULL); // Add event callback

      //touch call back
      //lv_obj_add_event_cb(sdcard_icon, display_static_image_png, LV_EVENT_CLICKED, NULL); // Add event callback

       //touch call back
       lv_obj_add_event_cb(sdcard_icon, go_to_screen4, LV_EVENT_CLICKED, NULL); // Add event callback
}