#ifndef UTILS_H
#define UTILS_H

// Standard C includes
#include "esp_timer.h" 
#include <nvs_flash.h>
#include <nvs.h>

//project specific includes
#include "esp_now_manager.h"
#include "screen3.h"
#include "globals.h"
#include "diner_font_set.h"


bool debounce(uint64_t *last_press, uint32_t delay_ms);
void nvs_init();
lv_color_t load_color_status();
void save_brightness(int brightness);
int32_t load_brightness();
void save_power_status(bool status);
bool load_power_status();
void save_color_status(lv_color_t color);
void make_font_styles(void);


// Other utility function prototypes...

#endif // UTILS_H