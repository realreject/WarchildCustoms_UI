#include "include.h"

// ESP error logging tag
static const char *TAG = "WARCHILD CUSTOMS";

// defines
#define STORAGE_NAMESPACE "storage"

// Flag status for ESP-NOW
static bool power_status = false;

// global variables
lv_obj_t *screen1, *screen2;

// function prototypes
void swipe_event_handler(lv_event_t *e);
static void power_btn_event_handler(lv_event_t *e);
void save_power_status(bool status);
bool load_power_status();
void ESP_NOW_START();
void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);
void onDataRecv(const esp_now_recv_info_t *recv_info, const uint8_t *data, int data_len);
static void wifi_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data);
void nvs_init();
void wifi_init();

uint8_t clientAddress[] = {0x0c, 0xb8, 0x15, 0xd8, 0x2f, 0xc8};

void setup();

#if !CONFIG_AUTOSTART_ARDUINO
void app_main()
{
      setup();
}
#endif

void setup()
{      
      nvs_init();
      wifi_init();
      initialize_system();

      //create screen 1
      screen1 = lv_obj_create(NULL);
      lv_obj_t *bg_img1 = lv_img_create(screen1);
      lv_img_set_src(bg_img1, &viking_bg_480_320);
      lv_obj_align(bg_img1, LV_ALIGN_CENTER, 0, 0);

      //create label for screen 1
      lv_obj_t *label1 = lv_label_create(screen1);
      lv_label_set_text(label1, "MAIN UI SCREEN");
      lv_obj_set_style_text_color(label1, lv_color_white(), 0);      // Set text color to white
      lv_obj_set_style_text_font(label1, &lv_font_montserrat_28, 0); // Set font size to larger font
      lv_obj_align(label1, LV_ALIGN_CENTER, 0, -50);

      //create screen 2
      screen2 = lv_obj_create(NULL);
      lv_obj_t *bg_img2 = lv_img_create(screen2);
      lv_img_set_src(bg_img2, &viking_bg_480_320);
      lv_obj_align(bg_img2, LV_ALIGN_CENTER, 0, 0);

      //create label for screen 2
      lv_obj_t *label2 = lv_label_create(screen2);
      lv_label_set_text(label2, "COLOR PICKER SCREEN");
      lv_obj_set_style_text_color(label2, lv_color_white(), 0);      // Set text color to white
      lv_obj_set_style_text_font(label2, &lv_font_montserrat_28, 0); // Set font size to larger font
      lv_obj_align(label2, LV_ALIGN_CENTER, 0, -50);

      // Create a power button as only the power symbol without background
      lv_obj_t *power_btn = lv_label_create(screen1);                   // Create a label as the button
      lv_label_set_text(power_btn, LV_SYMBOL_POWER);                    // Use the LVGL power symbol
      lv_obj_set_style_text_font(power_btn, &lv_font_montserrat_40, 0); // Match the font size to label1

      // Center the power button under the "MAIN UI SCREEN" label
      lv_coord_t label1_y = lv_obj_get_y(label1);

      // Set the position of the power button
      lv_obj_align(power_btn, LV_ALIGN_CENTER, 0, label1_y + lv_obj_get_height(label1) + 10); // Position the power button below the label

      // Make the power button interactive
      lv_obj_add_flag(power_btn, LV_OBJ_FLAG_CLICKABLE);
      lv_obj_add_event_cb(power_btn, power_btn_event_handler, LV_EVENT_ALL, NULL);

      // Apply any necessary styles to the symbol (if desired)
      if (power_status)
      {
            lv_obj_set_style_text_color(power_btn, lv_color_hex(0x00FF00), 0); // Set symbol color to green if "on"
      }
      else
      {
            lv_obj_set_style_text_color(power_btn, lv_color_hex(0xFF0000), 0); // Set symbol color to red if "off"
      }

      lv_obj_add_event_cb(screen1, swipe_event_handler, LV_EVENT_GESTURE, NULL);
      lv_obj_add_event_cb(screen2, swipe_event_handler, LV_EVENT_GESTURE, NULL);

      lv_scr_load(screen1);

      /* Release the mutex */
      bsp_display_unlock();

      ESP_NOW_START();
}

void loop()
{

      lv_timer_handler();           // Handle LVGL tasks
      lv_tick_inc(5);               // Increment LVGL tick count
      vTaskDelay(pdMS_TO_TICKS(5)); // Use FreeRTOS delay instead of delay(5)
}

void swipe_event_handler(lv_event_t *e)
{
      lv_event_code_t code = lv_event_get_code(e);
      if (code == LV_EVENT_GESTURE)
      {
            lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_get_act());
            if (dir == LV_DIR_LEFT)
            {
                  lv_scr_load_anim(screen2, LV_SCR_LOAD_ANIM_MOVE_LEFT, 500, 0, false);
            }
            else if (dir == LV_DIR_RIGHT)
            {
                  lv_scr_load_anim(screen1, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 500, 0, false);
            }
      }
}

// Button event handler
static void power_btn_event_handler(lv_event_t *e)
{
      lv_obj_t *obj = lv_event_get_target(e);
      lv_event_code_t event = lv_event_get_code(e);

      if (event == LV_EVENT_CLICKED)
      {
            // Change flag status for ESP-NOW
            power_status = !power_status;

            if (power_status)
            {
                  lv_obj_set_style_text_color(obj, lv_color_hex(0x00FF00), 0); // Change to green when "on"
                  ESP_LOGI(TAG, "Power button clicked, status: ON");
            }
            else
            {
                  lv_obj_set_style_text_color(obj, lv_color_hex(0xFF0000), 0); // Change to red when "off"
                  ESP_LOGI(TAG, "Power button clicked, status: OFF");
            }
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

void ESP_NOW_START()
{
      ESP_LOGI(TAG, "Initializing ESP-NOW");

      // Initialize WiFi
      wifi_init_config_t wifi_cfg = WIFI_INIT_CONFIG_DEFAULT();
      ESP_ERROR_CHECK(esp_wifi_init(&wifi_cfg));
      ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
      ESP_ERROR_CHECK(esp_wifi_start());

      // Initialize ESP-NOW
      ESP_ERROR_CHECK(esp_now_init());
      ESP_LOGI(TAG, "ESP-NOW initialized");

      // Register send and receive callback functions
      ESP_ERROR_CHECK(esp_now_register_send_cb(onDataSent));
      ESP_ERROR_CHECK(esp_now_register_recv_cb(onDataRecv));

      // Add peer
      esp_now_peer_info_t peerInfo = {};
      memcpy(peerInfo.peer_addr, clientAddress, 6);
      peerInfo.channel = 0; // Set the appropriate Wi-Fi channel
      peerInfo.encrypt = false;

      ESP_ERROR_CHECK(esp_now_add_peer(&peerInfo));
}

// Callback function that will be called when data is sent
void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status)
{
      ESP_LOGI(TAG, "Data sent to MAC: %02X:%02X:%02X:%02X:%02X:%02X, status: %s",
               mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5],
               status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
}

// Callback function that will be called when data is received
void onDataRecv(const esp_now_recv_info_t *recv_info, const uint8_t *data, int data_len)
{
      ESP_LOGI(TAG, "Data received from MAC: %02X:%02X:%02X:%02X:%02X:%02X, data length: %d",
               recv_info->src_addr[0], recv_info->src_addr[1], recv_info->src_addr[2],
               recv_info->src_addr[3], recv_info->src_addr[4], recv_info->src_addr[5], data_len);
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

void wifi_init()
{
      // Initialize WiFi
      ESP_ERROR_CHECK(esp_netif_init());
      ESP_ERROR_CHECK(esp_event_loop_create_default());
      esp_netif_create_default_wifi_sta();

      wifi_init_config_t wifi_cfg = WIFI_INIT_CONFIG_DEFAULT();
      ESP_ERROR_CHECK(esp_wifi_init(&wifi_cfg));

      ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL, NULL));
      ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_event_handler, NULL, NULL));

      wifi_config_t wifi_config = {
          .sta = {
              .ssid = WIFI_SSID,
              .password = WIFI_PASS,
              .threshold.authmode = WIFI_AUTH_WPA2_PSK,
          },
      };

      ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
      ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
      ESP_ERROR_CHECK(esp_wifi_start());
}

static void wifi_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
      if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START)
      {
            esp_wifi_connect();
      }
      else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED)
      {
            esp_wifi_connect();
            ESP_LOGI(TAG, "WiFi disconnected, trying to reconnect...");
      }
      else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP)
      {
            ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
            char ip_str[16];
            esp_ip4addr_ntoa(&event->ip_info.ip, ip_str, sizeof(ip_str));
            ESP_LOGI(TAG, "Got IP: %s", ip_str);
      }
}