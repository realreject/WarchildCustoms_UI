
#ifndef HOME_SCREEN_H
#define HOME_SCREEN_H

#include "globals.h"

void create_home_screen();
void create_bulb_button(lv_obj_t *parent);
void create_meter_button(lv_obj_t *parent);
void create_SDcard_button(lv_obj_t *parent);


void create_down_arrow(lv_obj_t *parent);
void create_right_arrow(lv_obj_t *parent);
void create_left_arrow(lv_obj_t *parent);


#endif // HOME_SCREEN_H