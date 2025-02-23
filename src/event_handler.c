// event_handler.c
#include "event_handler.h"
#include "include.h"
#include "globals.h"

// ESP error logging tag
static const char *TAG = "event_handler";

#define DEBOUNCE_DELAY_MS 200

// Define timestamp variables for debounce
uint64_t last_press_home = 0;
uint64_t last_press_bulb = 0;
uint64_t last_press_power = 0;
uint64_t last_press_color_wheel = 0;
uint64_t last_press_brightness_slider = 0;



// Button event handler
void power_btn_event_handler(lv_event_t *e)
{
      lv_obj_t *obj = lv_event_get_target(e);
      lv_event_code_t event = lv_event_get_code(e);

      uint32_t now = esp_log_timestamp(); // Get the current timestamp

      if (event == LV_EVENT_CLICKED && debounce(&last_press_power, DEBOUNCE_DELAY_MS))
      {
            last_press_power = now; // Update the last press timestamp

            // Change flag status for ESP-NOW
            power_status = !power_status;

            if (power_status)
            {
                  selected_color = load_color_status();
                  lv_obj_set_style_text_color(obj, selected_color, 0); // set color of power button

                  // Scale the RGB values based on the saved color
                  glow_red = (selected_color.ch.red * 255) / 31;
                  glow_green = (selected_color.ch.green_h * 255) / 63;
                  glow_blue = (selected_color.ch.blue * 255) / 31;

                  ESP_LOGI(TAG, "Power button clicked, status: ON");
                  glow_power = 1;
            }
            else
            {
                  lv_obj_set_style_text_color(obj, lv_color_hex(0x3c3c3c), 0); // Change to grey when "off"
                  ESP_LOGI(TAG, "Power button clicked, status: OFF");
                  glow_power = 0;
            }
            save_power_status(power_status);   // Save the power status
            save_color_status(selected_color); // Save the color status
            send_esp_data();
      }
}


// Event callback to handle brightness slider changes
void brightness_slider_event_cb(lv_event_t *e)
{
      if (debounce(&last_press_brightness_slider, DEBOUNCE_DELAY_MS))
      {
            lv_obj_t *slider = lv_event_get_target(e);
            // glow_brightness = lv_slider_get_value(slider);
            int new_brightness = lv_slider_get_value(slider);

            // Debug print
            ESP_LOGI("Brightness Slider", "New brightness value: %d", new_brightness);
            ESP_LOGI("Power Status", "Current power status: %d", power_status);

            if (power_status)
            {
                  glow_brightness = new_brightness;
                  ESP_LOGI("Brightness Slider", "Brightness set to: %d", glow_brightness);
                  send_esp_data();

                  // Save brightness level to NVS
                  save_brightness(glow_brightness);

                  // Update the slider's indicator and knob color to match the selected color
                  lv_obj_set_style_bg_color(slider, selected_color, LV_PART_INDICATOR);
                  lv_obj_set_style_bg_color(slider, selected_color, LV_PART_KNOB);
            }
            else
            {
                  // Ensure that the brightness is not changed when the power is off
                  if (new_brightness > 0)
                  {
                        glow_brightness = new_brightness;
                        ESP_LOGI("Brightness Slider", "Brightness set to: %d", glow_brightness);
                        send_esp_data();

                        // Save brightness level to NVS
                        save_brightness(glow_brightness);

                        // Update the slider's indicator and knob color to match the selected color
                        lv_obj_set_style_bg_color(slider, selected_color, LV_PART_INDICATOR);
                        lv_obj_set_style_bg_color(slider, selected_color, LV_PART_KNOB);
                  }
                  else
                  {
                        ESP_LOGW("Brightness Slider", "Ignored brightness adjustment because power is off and brightness value is 0.");
                  }
            }
      }
}

// Event callback to handle color changes
void color_wheel_event_cb(lv_event_t *e)
{
      if (debounce(&last_press_color_wheel, DEBOUNCE_DELAY_MS))
      {
            lv_obj_t *obj = lv_event_get_target(e);
            lv_color_t color = lv_colorwheel_get_rgb(obj);
            selected_color = color;

            // scale the RGB values to 0-255 8-bit range
            glow_red = (color.ch.red * 255) / 31;
            glow_green = (color.ch.green_h * 255) / 31;
            glow_blue = (color.ch.blue * 255) / 31;

            ESP_LOGI("Color Wheel", "Selected color: R=%d, G=%d, B=%d", glow_red, glow_green, glow_blue);

            // Update the slider's indicator and knob color
            lv_obj_set_style_bg_color(brightness_slider, selected_color, LV_PART_INDICATOR);
            lv_obj_set_style_bg_color(brightness_slider, selected_color, LV_PART_KNOB);

            // Update the power button color if it is "on"
            if (power_status)
            {
                  lv_obj_set_style_text_color(power_btn, selected_color, 0);
            }

            // Call the send_esp_data function to update the LED colors
            save_color_status(selected_color); //     Save the color status

            // Ensure that the LED state is correctly maintained
            glow_power = power_status ? 1 : 0;

            // Debugging log to verify the LED state
            ESP_LOGI("Color Wheel", "LED state: power_status=%d, glow_power=%d", power_status, glow_power);

            send_esp_data();
      }
}

void go_to_screen1(lv_event_t *e)
{
      if (debounce(&last_press_bulb, DEBOUNCE_DELAY_MS))
      {
            lv_scr_load(screen1); // Load the previously created screen1
      }
}

void go_to_screen2(lv_event_t *e)
{
      if (debounce(&last_press_home, DEBOUNCE_DELAY_MS))
      {
            lv_scr_load(screen2); // Load the previously created screen2
      }
}

void send_esp_data()
{
     // Change TAG to send_esp_data
     #define TAG "send_esp_data"

     // Scale the RGB values based on the brightness level
     float brightness_factor = glow_brightness / 255.0;
     myData.red = (int)(glow_red * brightness_factor);
     myData.green = (int)(glow_green * brightness_factor);
     myData.blue = (int)(glow_blue * brightness_factor);

     // Debug print for glow_power
     ESP_LOGI(TAG, "Current glow_power value: %d", glow_power);

     myData.power = glow_power;
     myData.led_brightness = glow_brightness;

     // Print the data being sent
     ESP_LOGI(TAG, "Preparing to send data: red=%d, green=%d, blue=%d, power=%d, brightness=%d",
              myData.red, myData.green, myData.blue, myData.power, myData.led_brightness);

     esp_err_t result = esp_now_send(clientAddress, (uint8_t *)&myData, sizeof(myData));

     if (result == ESP_OK)
     {
           ESP_LOGI(TAG, "result == ESP_OK -> Sent successfully");
     }
     else
     {
           ESP_LOGI(TAG, "Error sending");
     }
}
