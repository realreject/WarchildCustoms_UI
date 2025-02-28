
#ifndef SCREEN1_H
#define SCREEN1_H

#include "globals.h"
#include "viking_bg_480_320.h"
#include "common_ui.h"
#include "event_handler.h"

void create_screen1();
void create_bulb_button(lv_obj_t *parent);
void create_meter_button(lv_obj_t *parent);

#endif // SCREEN1_H