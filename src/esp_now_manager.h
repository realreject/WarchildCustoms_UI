#ifndef ESP_NOW_MANAGER_H
#define ESP_NOW_MANAGER_H

#include "include.h"  // Include the central header file for consistency

#define MAC_ADDR_LEN 6  // Define a constant for MAC address length


// ESP-NOW data structure
typedef struct message_struct
{
      int red;
      int green;
      int blue;
      int power;
      int led_brightness;
} message_struct;
 
extern message_struct myData;  // Declare the structure variable globally
extern uint8_t clientAddress[MAC_ADDR_LEN]; // Declare the MAC address of the client globally

void wifi_init();
void init_esp_now();
void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status); // Declare the onDataSent callback
void onDataRecv(const esp_now_recv_info_t *recv_info, const uint8_t *data, int data_len); // Declare the onDataRecv callback

#endif // ESP_NOW_MANAGER_H