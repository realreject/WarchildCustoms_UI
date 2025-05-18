#ifndef SYSTEM_INIT_H
#define SYSTEM_INIT_H

#include "lvgl.h"
#include "display.h"
#include "esp_bsp.h"

void initialize_system(); 
esp_err_t setupSDCard();
void preload_nvs_data();
void listFiles(const char *dirname, int numTabs);
void splash_screen();

#endif // SYSTEM_INIT_H
