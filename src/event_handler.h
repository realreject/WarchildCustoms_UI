// event_handler.h
#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include "include.h"

// Declare timestamp variables for debounce
extern uint64_t last_press_home;
extern uint64_t last_press_bulb;
extern uint64_t last_press_power;
extern uint64_t last_press_color_wheel;
extern uint64_t last_press_brightness_slider;

// Declare event handler functions
void power_btn_event_handler(lv_event_t *e);
void brightness_slider_event_cb(lv_event_t *e);
void color_wheel_event_cb(lv_event_t *e);
void go_to_screen2(lv_event_t *e);
void go_to_screen1(lv_event_t *e);
void send_esp_data();

#endif // EVENT_HANDLER_H
