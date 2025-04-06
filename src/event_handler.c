// event_handler.c
#include "event_handler.h"

// ESP error logging tag
static const char *TAG = "event_handler";

#define DEBOUNCE_DELAY_MS 50
#define POWER_BTN_DEBOUNCE_DELAY_MS 200
#define WORK_AREA_SIZE 4096 // Size of the work area for JPEG decoder

// for sd jpeg decode
#define JPEG_IMAGE_RGB565_SIZE (320 * 240 * 2) // Adjust dimensions as necessary

// Define timestamp variables for debounce
uint64_t last_press_home = 0;
uint64_t last_press_bulb = 0;
uint64_t last_press_meter = 0;
uint64_t last_press_power = 0;
uint64_t last_press_color_wheel = 0;
uint64_t last_press_brightness_slider = 0;
uint64_t last_press_SD = 0;

// Button event handler
void power_btn_event_handler(lv_event_t *e)
{
      lv_obj_t *obj = lv_event_get_target(e);
      lv_event_code_t event = lv_event_get_code(e);

      uint32_t now = esp_log_timestamp(); // Get the current timestamp

      if (event == LV_EVENT_CLICKED && debounce(&last_press_power, POWER_BTN_DEBOUNCE_DELAY_MS))
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

// Wrapper function for animation callback
void anim_img_angle_cb(void *var, int32_t value)
{
      lv_img_set_angle((lv_obj_t *)var, (int16_t)value);
}

void go_to_screen3(lv_event_t *e)
{
      if (debounce(&last_press_meter, DEBOUNCE_DELAY_MS))
      {
            lv_scr_load(screen3); // Load the previously created screen3

            // Uncomment one of the following lines for calibration purposes:
            // Set the needle to the start point for calibration
            // set_needle_to_start();

            // Set the needle to the stop point for calibration
            // set_needle_to_stop();

            // Create and start the animation
            // Adjust duration in Globals.h
            lv_anim_t a;
            lv_anim_init(&a);
            lv_anim_set_var(&a, pointer_needle);
            lv_anim_set_exec_cb(&a, anim_img_angle_cb);
            lv_anim_set_time(&a, ANIMATION_DURATION);                                   // Duration of the animation in ms
            lv_anim_set_values(&a, start_angle * 10, (start_angle + total_sweep) * 10); // Angles in 0.1 degrees
            lv_anim_set_path_cb(&a, lv_anim_path_ease_in_out);                          // Optional: set animation path
            lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);                      // Loop the animation
            lv_anim_set_playback_time(&a, ANIMATION_DURATION);                          // Optional: reverse animation
            lv_anim_start(&a);
      }
}

void go_to_screen4(lv_event_t *e)
{
      if (debounce(&last_press_home, DEBOUNCE_DELAY_MS))
      {
            lv_scr_load(screen4); // Load the previously created screen4
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



#include <stdio.h>
#include <stdlib.h>
//#include "esp_jpeg.h"
#include "esp_log.h"
#include "lvgl.h"

//#define TAG "JPEG_SD"

#define LV_HOR_RES_MAX 240
#define LV_VER_RES_MAX 320

// Function to decode and display JPEG from SD card using LVGL
void read_and_display_images(lv_event_t *event)
{
  /*   static const char *image_path = "/sdcard/test.jpg"; // Path to the image file

    ESP_LOGI(TAG, "Starting read_and_display_images function.");

    // Open the JPEG file
    FILE *file = fopen(image_path, "rb");
    if (!file) {
        ESP_LOGE(TAG, "Failed to open image file: %s", image_path);
        return;
    }

    // Get file size
    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file);
    rewind(file);

    ESP_LOGI(TAG, "JPEG file size: %zu bytes", file_size);

    // Allocate memory for the JPEG file data
    uint8_t *jpeg_data = malloc(file_size);
    if (!jpeg_data) {
        ESP_LOGE(TAG, "Failed to allocate memory for JPEG data.");
        fclose(file);
        return;
    }

    // Read the JPEG file into memory
    fread(jpeg_data, 1, file_size, file);
    fclose(file);

    // Allocate memory for the decoded image (RGB565 format)
    uint16_t *pixels = calloc(LV_HOR_RES_MAX * LV_VER_RES_MAX, sizeof(uint16_t)); // Adjust for display resolution
    if (!pixels) {
        ESP_LOGE(TAG, "Failed to allocate memory for pixel buffer.");
        free(jpeg_data);
        return;
    }

    // Set up JPEG decode configuration
    esp_jpeg_image_cfg_t jpeg_cfg = {
        .indata = jpeg_data,
        .indata_size = file_size,
        .outbuf = (uint8_t *)pixels,
        .outbuf_size = LV_HOR_RES_MAX * LV_VER_RES_MAX * sizeof(uint16_t),
        .out_format = JPEG_IMAGE_FORMAT_RGB565,
        .out_scale = JPEG_IMAGE_SCALE_0, // No scaling
        .flags = {.swap_color_bytes = 1}
    };

    esp_jpeg_image_output_t outimg;

    // Decode the JPEG file
    esp_err_t ret = esp_jpeg_decode(&jpeg_cfg, &outimg);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to decode JPEG image.");
        free(jpeg_data);
        free(pixels);
        return;
    }

    ESP_LOGI(TAG, "JPEG image decoded successfully: %dx%d pixels.", outimg.width, outimg.height);

    // Create an LVGL image descriptor for the decoded image
    static lv_img_dsc_t img_dsc;
    img_dsc.header.always_zero = 0;
    img_dsc.header.w = outimg.width;
    img_dsc.header.h = outimg.height;
    img_dsc.header.cf = LV_IMG_CF_TRUE_COLOR;
    img_dsc.data = (const uint8_t *)pixels;
    img_dsc.data_size = outimg.width * outimg.height * sizeof(uint16_t);

    // Create an LVGL image object to display the image
    lv_obj_t *img = lv_img_create(lv_scr_act()); // Add image to the active screen
    if (!img) {
        ESP_LOGE(TAG, "Failed to create LVGL image object.");
        free(jpeg_data);
        free(pixels);
        return;
    }

    lv_img_set_src(img, &img_dsc); // Set the image source for the LVGL object
    lv_obj_center(img);           // Center the image on the screen

    ESP_LOGI(TAG, "Image displayed successfully on LVGL.");

    // Clean up
    free(jpeg_data); // Free JPEG data
    // Do not free `pixels`, as LVGL uses it for rendering */
}