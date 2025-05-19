#ifndef NAV_BUTTONS_H
#define NAV_BUTTONS_H

#include <lvgl.h>

#define NUM_HORIZ_SCREENS 3

extern lv_obj_t *horizontal_screens[NUM_HORIZ_SCREENS];
extern int current_horizontal_index;
extern uint64_t last_press_nav;

void init_horizontal_screens(void);
void go_to_next_screen(lv_event_t *e);
void go_to_prev_screen(lv_event_t *e);
void create_horiz_nav_buttons(lv_obj_t *parent);
void create_vert_nav_buttons(lv_obj_t *parent);


#endif // NAV_BUTTONS_H