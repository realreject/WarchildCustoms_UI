
#ifndef HOME_SCREEN_H
#define HOME_SCREEN_H

#include "globals.h"
#include "common_ui.h"
#include "event_handler.h"
#include "set_bg.h"

//#include "viking_bg_480_320.h"


void create_home_screen();
void create_bulb_button(lv_obj_t *parent);
void create_meter_button(lv_obj_t *parent);
void create_SDcard_button(lv_obj_t *parent);


#endif // HOME_SCREEN_H