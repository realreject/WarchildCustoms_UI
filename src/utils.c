#include "include.h"

// defines
#define STORAGE_NAMESPACE "storage"

// ESP error logging tag
static const char *TAG = "utils";

// Debounce function
bool debounce(uint64_t *last_press, uint32_t delay_ms)
{
      uint64_t now = esp_timer_get_time() / 1000; // Current time in milliseconds
      if (now - *last_press > delay_ms)
      {
            *last_press = now;
            return true;
      }
      return false;
}

void nvs_init()
{
      // Initialize NVS
      esp_err_t err = nvs_flash_init();
      if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND)
      {
            ESP_ERROR_CHECK(nvs_flash_erase());
            err = nvs_flash_init();
      }
      ESP_ERROR_CHECK(err);
}

lv_color_t load_color_status()
{
      nvs_handle_t nvs_handle;
      esp_err_t err = nvs_open(STORAGE_NAMESPACE, NVS_READONLY, &nvs_handle);
      lv_color_t color = {0};
      if (err == ESP_OK)
      {
            uint8_t red, green, blue;
            if (nvs_get_u8(nvs_handle, "red", &red) == ESP_OK &&
                nvs_get_u8(nvs_handle, "green", &green) == ESP_OK &&
                nvs_get_u8(nvs_handle, "blue", &blue) == ESP_OK)
            {
                  color.ch.red = red;
                  color.ch.green_h = green;
                  color.ch.blue = blue;
            }
            nvs_close(nvs_handle);
      }
      else
      {
            ESP_LOGI(TAG, "Error reading NVS");
      }
      return color;
}

void save_brightness(int brightness)
{
      nvs_handle_t nvs_handle;
      esp_err_t err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &nvs_handle);
      if (err == ESP_OK)
      {
            nvs_set_i32(nvs_handle, "brightness", brightness);
            nvs_commit(nvs_handle);
            nvs_close(nvs_handle);
      }
      else
      {
            ESP_LOGI(TAG, "Error opening NVS");
      }
}

int32_t load_brightness()
{
      nvs_handle_t nvs_handle;
      esp_err_t err = nvs_open(STORAGE_NAMESPACE, NVS_READONLY, &nvs_handle);
      int32_t brightness = 255 / 2; // Default to half brightness
      if (err == ESP_OK)
      {
            if (nvs_get_i32(nvs_handle, "brightness", &brightness) != ESP_OK)
            {
                  ESP_LOGI(TAG, "Error reading brightness from NVS");
            }
            nvs_close(nvs_handle);
      }
      else
      {
            ESP_LOGI(TAG, "Error opening NVS");
      }
      return brightness;
}

// Function to save the power status to NVS
void save_power_status(bool status)
{
      nvs_handle_t nvs_handle;
      esp_err_t err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &nvs_handle);
      if (err == ESP_OK)
      {
            nvs_set_u8(nvs_handle, "power_status", status ? 1 : 0);
            nvs_commit(nvs_handle);
            nvs_close(nvs_handle);
      }
      else
      {
            ESP_LOGI(TAG, "Error opening NVS");
      }
}

// Function to load the power status from NVS
bool load_power_status()
{
      nvs_handle_t nvs_handle;
      esp_err_t err = nvs_open(STORAGE_NAMESPACE, NVS_READONLY, &nvs_handle);
      if (err == ESP_OK)
      {
            uint8_t status;
            if (nvs_get_u8(nvs_handle, "power_status", &status) == ESP_OK)
            {
                  nvs_close(nvs_handle);
                  // Add a debug print statement to see the status value
                  ESP_LOGI(TAG, "Loaded power status from NVS: %d", status);
                  return status == 1;
            }
            nvs_close(nvs_handle);
      }
      ESP_LOGI(TAG, "Error reading NVS");
      return false; // Default to off if reading fails
}

void save_color_status(lv_color_t color)
{
      nvs_handle_t nvs_handle;
      esp_err_t err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &nvs_handle);
      if (err == ESP_OK)
      {
            nvs_set_u8(nvs_handle, "red", color.ch.red);
            nvs_set_u8(nvs_handle, "green", color.ch.green_h);
            nvs_set_u8(nvs_handle, "blue", color.ch.blue);
            nvs_commit(nvs_handle);
            nvs_close(nvs_handle);
      }
      else
      {
            ESP_LOGI(TAG, "Error opening NVS");
      }
}


// Other utility functions...