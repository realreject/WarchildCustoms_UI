#include "include.h"
#include "globals.h"

// ESP error logging tag
//static const char *TAG = "main.c";

// Function prototypes
void lvgl_task(void *pvParameter);
// void send_esp_data();

// Main function
void app_main()
{
      // Initialize NVS
      nvs_init();

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

      create_screen1();
      create_screen2();

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
