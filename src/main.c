#include <lvgl.h>
#include <esp_log.h>

#include "globals.h"
#include "utils.h"
#include "system_init.h"
#include "screen1.h"
#include "screen2.h"
#include "screen3.h"
#include "screen4.h"

// ESP error logging tag
static const char *TAG = "main.c";

// Function prototypes
void lvgl_task(void *pvParameter);

// Main function
void app_main()
{
    // Set up the SD card w/ NVS initialization
    esp_err_t ret = setupSDCard();
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to set up SD card");
    }

    // Load saved power status, color, and brightness
    power_status = load_power_status();
    selected_color = load_color_status();
    glow_brightness = load_brightness();

    // Set glow_power based on the loaded power status
    glow_power = power_status ? 1 : 0;

    // Initialize ESP-NOW
    init_esp_now();

    // Initialize the rest of the system
    initialize_system();
    make_font_styles();

    ESP_LOGI(TAG, "Initialization functions complete");

    create_screen1();
    create_screen2();
    create_screen3();
    create_screen4();

    ESP_LOGI(TAG, "Screens created");

    lv_scr_load(screen1);

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
