// event_handler.h
#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include "include.h"


// Declare event handler functions
void power_btn_event_handler(lv_event_t *e);
void brightness_slider_event_cb(lv_event_t *e);
void color_wheel_event_cb(lv_event_t *e);
void go_to_screen1(lv_event_t *e);
void go_to_screen2(lv_event_t *e);
void go_to_screen3(lv_event_t *e);
void update_meter_value(int32_t kmh_value);
void demo_update_meter(lv_timer_t *timer);
void update_rpm_value(int32_t rpm_value);
void demo_update_rpm(lv_timer_t *timer);
void send_esp_data();

// Declare timestamp variables for debounce
extern uint64_t last_press_home;
extern uint64_t last_press_bulb;
extern uint64_t last_press_meter;
extern uint64_t last_press_power;
extern uint64_t last_press_color_wheel;
extern uint64_t last_press_brightness_slider;

#endif // EVENT_HANDLER_H