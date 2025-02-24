#include "include.h"
#include "globals.h"

// ESP error logging tag
// static const char *TAG = "main.c";

lv_obj_t *fps_label_screen1 = NULL;
lv_obj_t *fps_label_screen2 = NULL;
lv_obj_t *fps_label_screen3 = NULL;

// Function prototypes
void lvgl_task(void *pvParameter);


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
      create_screen3();  

      setup_and_update_fps(screen1, &fps_label_screen1);
      setup_and_update_fps(screen2, &fps_label_screen2);
      setup_and_update_fps(screen3, &fps_label_screen3);

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
  
          lv_obj_t *active_screen = lv_scr_act();
          if (active_screen == screen1)
          {
              setup_and_update_fps(screen1, &fps_label_screen1);
          }
          else if (active_screen == screen2)
          {
              setup_and_update_fps(screen2, &fps_label_screen2);
          }
          else if (active_screen == screen3)
          {
              setup_and_update_fps(screen3, &fps_label_screen3);
          }
      }
}

