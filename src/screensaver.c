#include "screensaver.h"
#include "globals.h"
#include "esp_bsp.h"
#include "esp_log.h"
#include "esp_timer.h"


static const char *TAG = "Screensaver";
static esp_timer_handle_t inactivity_timer;


void screensaver_timer_callback(void *arg) {
    ESP_LOGI(TAG, "Starting screen dimming...");
    for (int brightness = SCREEN_BRIGHTNESS_MAX; brightness >= SCREEN_BRIGHTNESS_DIM; brightness -= 5) {
        bsp_display_brightness_set(brightness);
        screensaver_active = true; // Set the screensaver active flag
        ESP_LOGI(TAG, "Dimming screen to %d%% brightness.", brightness);
        vTaskDelay(pdMS_TO_TICKS(100)); // Gradual fade effect
    }
}

void screensaver_init(void) {
    esp_timer_create_args_t timer_args = {
        .callback = &screensaver_timer_callback,
        .name = "screensaver_timer"
    };
    esp_timer_create(&timer_args, &inactivity_timer);
}

void screensaver_reset_timer(void) {
    esp_timer_stop(inactivity_timer);
    esp_timer_start_once(inactivity_timer, INACTIVITY_TIMEOUT * 1000000);
}

void screensaver_restore_brightness(void) {
    ESP_LOGI(TAG, "Restoring full brightness.");
    esp_timer_stop(inactivity_timer);
    bsp_display_brightness_set(SCREEN_BRIGHTNESS_MAX);
}