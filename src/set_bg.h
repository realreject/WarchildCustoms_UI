#ifndef SET_BG_H
#define SET_BG_H

#include <lvgl.h>
#include "globals.h"
#include "event_handler.h"
#include "viking_bg_480_320.h"
#include <esp_log.h>


//testng
void initialize_global_bg(lv_obj_t *parent);
bool nvs_get_background_file(char *file_path, size_t max_size);
void attach_bg_to_screen(lv_obj_t *screen);
void update_global_bg(const char *file_path);
void set_png_image(const char *file_path);

/////////////

bool is_file_valid(const char *file_path);
bool save_background_to_nvs(const char *file_path);
bool validate_image_with_decoder(const char *file_path);




#endif // SET_BG_H