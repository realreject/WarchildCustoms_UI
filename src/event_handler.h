// event_handler.h
#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

//standard includes
#include <lvgl.h>
#include <esp_log.h>

//project includes
#include "globals.h"
#include "config.h"
#include "globals.h"
#include "utils.h"

//for SD card
#include "driver/sdspi_host.h"
#include "driver/spi_common.h"
#include "sdmmc_cmd.h"
#include "esp_vfs_fat.h"
#include <string.h>
#include "ff.h"           // Include FatFS header for file operations
#include "esp_log.h"      // ESP-IDF logging functionality

/* //for image handling
#include <dirent.h>  // Include this header for directory handling functions
#include <string.h>  // Include this header for string handling functions
#include <stdio.h>
#include <stdlib.h> */


extern lv_timer_t *needle_timer;

// Declare event handler functions
void power_btn_event_handler(lv_event_t *e);
void brightness_slider_event_cb(lv_event_t *e);
void color_wheel_event_cb(lv_event_t *e);
void go_to_screen1(lv_event_t *e);
void go_to_screen2(lv_event_t *e);
void anim_img_angle_cb(void * var, int32_t value);
void go_to_screen3(lv_event_t *e);
void go_to_screen4(lv_event_t *e);
void send_esp_data();

//SD card testing
void read_and_display_images(lv_event_t *event);
//static int jpeg_output_callback(JDEC *jd, void *bitmap, JRECT *rect);
//static size_t jpeg_data_infunc(JDEC *jd, uint8_t *buffer, size_t len);
//void display_static_image_png(const char *image_path);


// Declare timestamp variables for debounce
extern uint64_t last_press_home;
extern uint64_t last_press_bulb;
extern uint64_t last_press_meter;
extern uint64_t last_press_power;
extern uint64_t last_press_color_wheel;
extern uint64_t last_press_brightness_slider;

#endif // EVENT_HANDLER_H