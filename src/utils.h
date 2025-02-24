#ifndef UTILS_H
#define UTILS_H

#include "include.h"

bool debounce(uint64_t *last_press, uint32_t delay_ms);
void nvs_init();
lv_color_t load_color_status();
void save_brightness(int brightness);
int32_t load_brightness();
void save_power_status(bool status);
bool load_power_status();
void save_color_status(lv_color_t color);
void setup_and_update_fps(lv_obj_t *screen, lv_obj_t **fps_label);

// Other utility function prototypes...

#endif // UTILS_H