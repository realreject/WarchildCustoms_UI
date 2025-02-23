#include "include.h"

// ESP error logging tag
static const char *TAG = "WARCHILD CUSTOMS";

// defines
#define STORAGE_NAMESPACE "storage"
#define TITLE_FONT_SIZE &lv_font_montserrat_28
#define ICON_FONT_SIZE &lv_font_montserrat_48
#define DEBOUNCE_DELAY_MS 200

// Flag status for ESP-NOW
static bool power_status = false;

// global variables
lv_obj_t *screen1, *screen2, *color_wheel, *brightness_slider, *power_btn;
lv_color_t selected_color;

// function prototypes
void swipe_event_handler(lv_event_t *e);
static void power_btn_event_handler(lv_event_t *e);
void save_power_status(bool status);
bool load_power_status();
void nvs_init();
void lvgl_task(void *pvParameter);
void send_esp_data();
void create_color_wheel(lv_obj_t *parent);
void color_wheel_event_cb(lv_event_t *e);
void brightness_slider_event_cb(lv_event_t *e);
void go_to_screen2(lv_event_t *e);
void go_to_screen1(lv_event_t *e);
void create_bulb_button(lv_obj_t *parent);
void create_home_button(lv_obj_t *parent);
bool debounce(uint64_t *last_press, uint32_t delay_ms);
void save_color_status(lv_color_t color);
lv_color_t load_color_status();
int load_brightness();
void save_brightness(int brightness);

// Define default values for glow data
int glow_red = 255;            // Default red value
int glow_green = 255;          // Default green value
int glow_blue = 255;           // Default blue value
int glow_power = 0;            // Default power state
int glow_brightness = 20; // Default brightness value

// Timestamp variables for debounce
static uint64_t last_press_home = 0;
static uint64_t last_press_bulb = 0;
static uint64_t last_press_power = 0;
static uint64_t last_press_color_wheel = 0;
static uint64_t last_press_brightness_slider = 0;

// Main function
void app_main()
{
      // Initialize NVS
      nvs_init();
      ESP_LOGI(TAG, "NVS initialized");

      // Load saved power status, color, and brightness
      power_status = load_power_status();
      selected_color = load_color_status();
      glow_brightness = load_brightness();

      // Initialize ESP-NOW
      init_esp_now();
      ESP_LOGI(TAG, "ESP-NOW initialized");

      // Initialize the rest of the system
      initialize_system();
      ESP_LOGI(TAG, "System initialized");

      // create screen 1
      screen1 = lv_obj_create(NULL);
      lv_obj_t *bg_img1 = lv_img_create(screen1);
      lv_img_set_src(bg_img1, &viking_bg_480_320);
      lv_obj_align(bg_img1, LV_ALIGN_CENTER, 0, 0);

      // create label for screen 1
      lv_obj_t *label1 = lv_label_create(screen1);
      lv_label_set_text(label1, "MEGA CAB UI");
      lv_obj_set_style_text_color(label1, lv_color_white(), 0); // Set text color to white
      lv_obj_set_style_text_font(label1, TITLE_FONT_SIZE, 0);   // Set font size to larger font
      lv_obj_align(label1, LV_ALIGN_CENTER, 0, -135);

      // Create the bulb button on screen 1
      create_bulb_button(screen1);

      // create screen 2
      screen2 = lv_obj_create(NULL);
      lv_obj_t *bg_img2 = lv_img_create(screen2);
      lv_img_set_src(bg_img2, &viking_bg_480_320);
      lv_obj_align(bg_img2, LV_ALIGN_CENTER, 0, 0);

      // create label for screen 2
      lv_obj_t *label2 = lv_label_create(screen2);
      lv_label_set_text(label2, "FOOTWELL LAMPS");
      lv_obj_set_style_text_color(label2, lv_color_white(), 0); // Set text color to white
      lv_obj_set_style_text_font(label2, TITLE_FONT_SIZE, 0);   // Set font size to larger font
      lv_obj_align(label2, LV_ALIGN_CENTER, 0, -135);

      // Create the color wheel and add it to screen 2
      create_color_wheel(screen2);

      // Set the initial value of the color wheel to the saved color
      lv_colorwheel_set_rgb(color_wheel, selected_color);

      // Create a power button as only the power symbol without background
      power_btn = lv_label_create(screen2);          // Create a label as the button
      lv_label_set_text(power_btn, LV_SYMBOL_POWER); // Use the LVGL power symbol
      lv_obj_set_style_text_font(power_btn, ICON_FONT_SIZE, 0);
      lv_obj_align(power_btn, LV_ALIGN_CENTER, 0, 0);

      // Make the power button interactive
      lv_obj_add_flag(power_btn, LV_OBJ_FLAG_CLICKABLE);
      lv_obj_add_event_cb(power_btn, power_btn_event_handler, LV_EVENT_ALL, NULL);

      // Use selected color for the power button when "on"
      // lv_color_t selected_color = lv_color_make(glow_red, glow_green, glow_blue);

      // Apply any necessary styles to the symbol (if desired)
      if (power_status)
      {
            lv_obj_set_style_text_color(power_btn, selected_color, 0); // Set symbol color to green if "on"
      }
      else
      {
            lv_obj_set_style_text_color(power_btn, lv_color_hex(0x3c3c3c), 0); // Set symbol color to red if "off"
      }

      // Create the home button on screen 2
      create_home_button(screen2);

      // Create a slider bar below the color wheel to control the brightness
      brightness_slider = lv_slider_create(screen2);
      lv_obj_set_width(brightness_slider, 200);                 // Set width of the slider
      lv_obj_align(brightness_slider, LV_ALIGN_CENTER, 0, 140); // Align the slider below the color wheel
      lv_obj_add_event_cb(brightness_slider, brightness_slider_event_cb, LV_EVENT_VALUE_CHANGED, NULL);

      // Set the initial value of the slider
      lv_slider_set_range(brightness_slider, 0, 255);
      lv_slider_set_value(brightness_slider, glow_brightness, LV_ANIM_OFF);

      // Update the slider's indicator and knob color based on saved color
      lv_obj_set_style_bg_color(brightness_slider, selected_color, LV_PART_INDICATOR);
      lv_obj_set_style_bg_color(brightness_slider, selected_color, LV_PART_KNOB);

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

// Button event handler
static void power_btn_event_handler(lv_event_t *e)
{
      lv_obj_t *obj = lv_event_get_target(e);
      lv_event_code_t event = lv_event_get_code(e);

      uint32_t now = esp_log_timestamp(); // Get the current timestamp

      if (event == LV_EVENT_CLICKED && debounce(&last_press_power, DEBOUNCE_DELAY_MS))
      {
            last_press_power = now; // Update the last press timestamp

            // Change flag status for ESP-NOW
            power_status = !power_status;

            // Use selected color for the power button when "on"
            lv_color_t selected_color = lv_colorwheel_get_rgb(color_wheel);

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
                  return status == 1;
            }
            nvs_close(nvs_handle);
      }
      ESP_LOGI(TAG, "Error reading NVS");
      return false; // Default to off if reading fails
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

void send_esp_data()
{
      // Scale the RGB values based on the brightness level
      float brightness_factor = glow_brightness / 255.0;
      myData.red = (int)(glow_red * brightness_factor);
      myData.green = (int)(glow_green * brightness_factor);
      myData.blue = (int)(glow_blue * brightness_factor);
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

// Event callback to handle brightness slider changes
void brightness_slider_event_cb(lv_event_t *e)
{
      if (debounce(&last_press_brightness_slider, DEBOUNCE_DELAY_MS))
      {
            lv_obj_t *slider = lv_event_get_target(e);
            glow_brightness = lv_slider_get_value(slider);
            ESP_LOGI("Brightness Slider", "Brightness set to: %d", glow_brightness);
            send_esp_data();

            // Save brightness level to NVS
            save_brightness(glow_brightness);

            // Update the slider's indicator and knob color to match the selected color
            lv_obj_set_style_bg_color(slider, selected_color, LV_PART_INDICATOR);
            lv_obj_set_style_bg_color(slider, selected_color, LV_PART_KNOB);
      }
}

// Function to create the color wheel and handle color changes
void create_color_wheel(lv_obj_t *parent)
{
      color_wheel = lv_colorwheel_create(parent, true);
      lv_obj_set_size(color_wheel, 200, 200);           // Set size of the color wheel
      lv_obj_align(color_wheel, LV_ALIGN_CENTER, 0, 0); // Align the color wheel in the center of the parent

      // Adjust the line width to make the arc/circle wider
      lv_obj_set_style_arc_width(color_wheel, 20, 0); // Set the arc width to 20 (adjust as needed)

      // Add an event callback to handle color changes
      lv_obj_add_event_cb(color_wheel, color_wheel_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
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
            send_esp_data();
      }
}

void go_to_screen2(lv_event_t *e)
{
      if (debounce(&last_press_home, DEBOUNCE_DELAY_MS))
      {
            lv_scr_load(screen2); // Load the previously created screen2
      }
}

void go_to_screen1(lv_event_t *e)
{
      if (debounce(&last_press_bulb, DEBOUNCE_DELAY_MS))
      {
            lv_scr_load(screen1); // Load the previously created screen1
      }
}

void create_bulb_button(lv_obj_t *parent)
{
      lv_obj_t *bulb_icon = lv_label_create(parent);            // Create a label for the icon
      lv_label_set_text(bulb_icon, LV_SYMBOL_EYE_OPEN);         // Set the text to the home symbol
      lv_obj_set_style_text_font(bulb_icon, ICON_FONT_SIZE, 0); // Set the font size to match the power icon
      lv_obj_align(bulb_icon, LV_ALIGN_CENTER, -180, -75);      // Center the icon with an offset

      // Make the icon interactive
      lv_obj_add_flag(bulb_icon, LV_OBJ_FLAG_CLICKABLE);
      lv_obj_add_event_cb(bulb_icon, go_to_screen1, LV_EVENT_CLICKED, NULL); // Add event callback

      // Add styles to the icon
      lv_obj_set_style_text_color(bulb_icon, lv_color_white(), 0); // Set text color

      lv_obj_add_event_cb(bulb_icon, go_to_screen2, LV_EVENT_CLICKED, NULL); // Add event callback
}

void create_home_button(lv_obj_t *parent)
{
      lv_obj_t *home_icon = lv_label_create(parent);            // Create a label for the icon
      lv_label_set_text(home_icon, LV_SYMBOL_HOME);             // Set the text to the home symbol
      lv_obj_set_style_text_font(home_icon, ICON_FONT_SIZE, 0); // Set the font size to match the power icon
      lv_obj_align(home_icon, LV_ALIGN_CENTER, -180, 120);      // Center the icon with an offset

      // Make the icon interactive
      lv_obj_add_flag(home_icon, LV_OBJ_FLAG_CLICKABLE);
      lv_obj_add_event_cb(home_icon, go_to_screen1, LV_EVENT_CLICKED, NULL); // Add event callback

      // Add styles to the icon
      lv_obj_set_style_text_color(home_icon, lv_color_white(), 0); // Set text color

      lv_obj_add_event_cb(home_icon, go_to_screen1, LV_EVENT_CLICKED, NULL); // Add event callback
}

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

int load_brightness()
{
      nvs_handle_t nvs_handle;
      esp_err_t err = nvs_open(STORAGE_NAMESPACE, NVS_READONLY, &nvs_handle);
      int brightness = 255 / 2; // Default to half brightness
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
