#include <lvgl.h>
#include <esp_log.h>

#include "globals.h"
#include "utils.h"
#include "system_init.h"
#include "screensaver.h"
#include "home_screen.h"
#include "led_controls_screen.h"
#include "meter_screen.h"
#include "bg_sel_screen.h"
#include "set_bg.h"

// ESP error logging tag
static const char *TAG = "main.c";

// Function prototypes
void lvgl_task(void *pvParameter);

// Main function
void app_main()
{
    // Initialize of the system
    initialize_system();  

    // Set up the SD card w/ NVS initialization
    esp_err_t ret = setupSDCard();
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to set up SD card");
    }

    // default led colors and status
    preload_nvs_data();

    // Load saved power status, color, and brightness
    power_status = load_power_status();
    selected_color = load_color_status();
    glow_brightness = load_brightness();

    // Set glow_power based on the loaded power status
    glow_power = power_status ? 1 : 0;

    // Initialize ESP-NOW
    init_esp_now();   

    // initialize screensaver
    screensaver_init();
    screensaver_reset_timer(); // Start the timer for the screensaver

    // make custom font styles
    make_font_styles();
    
    ESP_LOGI(TAG, "Initialization functions complete");

    // Create the LVGL screens
    create_home_screen();
    lv_scr_load(home_screen);
    create_led_controls_screen();
    create_meter_screen(); // meters does not use the global background image
    create_bg_sel_screen();

    ESP_LOGI(TAG, "Screens created");

    /* Release the mutex */
    bsp_display_unlock();

    // Create LVGL task
    xTaskCreate(lvgl_task, "lvgl_task", 4096, NULL, 5, NULL);
}

void lvgl_task(void *pvParameter)
{
    while (1)
    {
        lv_timer_handler();
        lv_tick_inc(5);
        vTaskDelay(pdMS_TO_TICKS(5));
    }
}
