#ifndef SYSTEM_INIT_H
#define SYSTEM_INIT_H

#include "display.h"
#include "esp_bsp.h"
#include <esp_log.h>   
#include <esp_flash.h>
#include <esp_chip_info.h>

//for SD card
#include <stdio.h>
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"
#include "driver/sdspi_host.h"
#include "driver/spi_common.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "dirent.h"


// Define GPIO pins for SD card
#define SD_MMC_D0 13
#define SD_MMC_CLK 12
#define SD_MMC_CMD 11
#define SD_MMC_CS 10 // Chip select pin


void initialize_system();
esp_err_t setupSDCard();
void listFiles(const char *dirname, int numTabs);


#endif // SYSTEM_INIT_H
