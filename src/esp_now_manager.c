#include "esp_now_manager.h"

#define TAG "ESP_NOW_MANAGER" // Define the TAG for logging     

message_struct myData;// Define the structure variable
uint8_t clientAddress[MAC_ADDR_LEN] = {0x0c, 0xb8, 0x15, 0xd8, 0x2f, 0xc8}; // Initialize the MAC address of the client

// Callback function that will be called when data is sent
void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status)
{
    ESP_LOGI(TAG, "Data sent to MAC: %02X:%02X:%02X:%02X:%02X:%02X, status: %s",
             mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5],
             status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");

    if (status != ESP_NOW_SEND_SUCCESS)
    {
        ESP_LOGI(TAG, "Retrying...");
        vTaskDelay(pdMS_TO_TICKS(100)); // Small delay before retry
        esp_err_t result = esp_now_send(clientAddress, (uint8_t *)&myData, sizeof(myData));
        ESP_LOGI(TAG, "%s", result == ESP_OK ? "Retry Send Success" : "Retry Send Fail");
    }
}

// Callback function that will be called when data is received
void onDataRecv(const esp_now_recv_info_t *recv_info, const uint8_t *data, int data_len)
{
    ESP_LOGI(TAG, "Data received from MAC: %02X:%02X:%02X:%02X:%02X:%02X, data length: %d",
             recv_info->src_addr[0], recv_info->src_addr[1], recv_info->src_addr[2],
             recv_info->src_addr[3], recv_info->src_addr[4], recv_info->src_addr[5], data_len);
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

void wifi_init()
{
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

void init_esp_now()
{
    ESP_LOGI(TAG, "Initializing ESP-NOW");

    // Initialize Wi-Fi
    wifi_init();
    ESP_LOGI(TAG, "Wi-Fi initialized");

    ESP_ERROR_CHECK(esp_now_init());
    ESP_LOGI(TAG, "ESP-NOW initialized");

    ESP_ERROR_CHECK(esp_now_register_send_cb(onDataSent));
    ESP_ERROR_CHECK(esp_now_register_recv_cb(onDataRecv));

    esp_now_peer_info_t peerInfo = {};
    memcpy(peerInfo.peer_addr, clientAddress, 6);
    peerInfo.channel = 0; // Set the appropriate Wi-Fi channel
    peerInfo.encrypt = false;

    ESP_ERROR_CHECK(esp_now_add_peer(&peerInfo));
}
