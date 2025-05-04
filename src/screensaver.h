#ifndef SCREENSAVER_H
#define SCREENSAVER_H

#include "esp_timer.h"

// Define brightness levels
#define SCREEN_BRIGHTNESS_MAX 100
#define SCREEN_BRIGHTNESS_DIM 10

// Define inactivity timeout (seconds)
#define INACTIVITY_TIMEOUT 20

// 
void screensaver_init(void);
void screensaver_reset_timer(void);
void screensaver_start_dimming(void);
void screensaver_restore_brightness(void);



#endif // SCREENSAVER_H