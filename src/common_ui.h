#ifndef COMMON_UI_H
#define COMMON_UI_H

#include <lvgl.h>
#include <esp_log.h>
#include "event_handler.h"
#include "globals.h"

// Declare function to create home button
void create_home_button(lv_obj_t *parent);
void create_title(lv_obj_t *parent, const char *text, uint16_t x, uint16_t y);

void button_press_effect(lv_event_t *e);
void button_release_effect(lv_event_t *e);


#endif // COMMON_UI_H