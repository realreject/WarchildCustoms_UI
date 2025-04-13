#ifndef BG_SEL_SCREEN_H
#define BG_SEL_SCREEN_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "globals.h"
#include "common_ui.h"

#include <set_bg.h>



void create_bg_sel_screen(); 
void display_png_image(lv_obj_t *parent, const char *file_path);
void populate_dropdown_with_png_files(lv_obj_t *dropdown);
void dropdown_event_handler(lv_event_t *e);
void display_png_in_window(lv_obj_t *window, const char *file_path);
void to_lowercase(char *str);


#endif // BIG_SEL_SCREEN_H