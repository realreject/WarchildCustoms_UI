#include "system_init.h"
#include "lvgl.h"
#include <stdio.h>

/**
 * @brief LVGL porting example
 * Set the rotation degree:
 *      - 0: 0 degree
 *      - 90: 90 degree
 *      - 180: 180 degree
 *      - 270: 270 degree
 *
 */
#define LVGL_PORT_ROTATION_DEGREE (270)

void initialize_system()
{
    static const char *TAG = "initialize_system";

    esp_chip_info_t chip_info;
    uint32_t flash_size;
    esp_chip_info(&chip_info);
    ESP_LOGI(TAG, "This is %s chip with %d CPU core(s), %s%s%s%s, ",
             CONFIG_IDF_TARGET,
             chip_info.cores,
             (chip_info.features & CHIP_FEATURE_WIFI_BGN) ? "WiFi/" : "",
             (chip_info.features & CHIP_FEATURE_BT) ? "BT" : "",
             (chip_info.features & CHIP_FEATURE_BLE) ? "BLE" : "",
             (chip_info.features & CHIP_FEATURE_IEEE802154) ? ", 802.15.4 (Zigbee/Thread)" : "");

    unsigned major_rev = chip_info.revision / 100;
    unsigned minor_rev = chip_info.revision % 100;
    ESP_LOGI(TAG, "silicon revision v%d.%d, ", major_rev, minor_rev);
    if (esp_flash_get_size(NULL, &flash_size) != ESP_OK)
    {
        ESP_LOGI(TAG, "Get flash size failed");
        return;
    }

    ESP_LOGI(TAG, "%" PRIu32 "MB %s flash", flash_size / (uint32_t)(1024 * 1024),
             (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");

    ESP_LOGI(TAG, "Minimum free heap size: %" PRIu32 " bytes", esp_get_minimum_free_heap_size());
    size_t freePsram = heap_caps_get_free_size(MALLOC_CAP_SPIRAM);
    ESP_LOGI(TAG, "Free PSRAM: %d bytes", freePsram);

    bsp_display_cfg_t cfg = {
        .lvgl_port_cfg = ESP_LVGL_PORT_INIT_CONFIG(),
        .buffer_size = EXAMPLE_LCD_QSPI_H_RES * EXAMPLE_LCD_QSPI_V_RES,
#if LVGL_PORT_ROTATION_DEGREE == 90
        .rotate = LV_DISP_ROT_90,
#elif LVGL_PORT_ROTATION_DEGREE == 270
        .rotate = LV_DISP_ROT_270,
#elif LVGL_PORT_ROTATION_DEGREE == 180
        .rotate = LV_DISP_ROT_180,
#elif LVGL_PORT_ROTATION_DEGREE == 0
        .rotate = LV_DISP_ROT_NONE,
#endif
    };

    bsp_display_start_with_config(&cfg);
    bsp_display_backlight_on();

    /* Lock the mutex due to the LVGL APIs are not thread-safe */
    bsp_display_lock(0);

    // Unlock the LVGL mutex
    bsp_display_unlock();
}




esp_err_t setupSDCard()
{
    static const char *TAG = "setup SD Card";

    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    ESP_LOGI(TAG, "Initializing SD card");

    sdmmc_host_t host = SDSPI_HOST_DEFAULT();
    host.slot = SPI3_HOST; // Use appropriate SPI host
    host.flags = SDMMC_HOST_FLAG_SPI;
    host.max_freq_khz = SDMMC_FREQ_DEFAULT;

    spi_bus_config_t bus_cfg = {
        .mosi_io_num = SD_MMC_CMD,
        .miso_io_num = SD_MMC_D0,
        .sclk_io_num = SD_MMC_CLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 4000,
        .flags = 0,
        .intr_flags = 0,
    };

    sdspi_device_config_t slot_config = SDSPI_DEVICE_CONFIG_DEFAULT();
    slot_config.gpio_cs = SD_MMC_CS;
    slot_config.host_id = host.slot;

    gpio_set_pull_mode(SD_MMC_CMD, GPIO_PULLUP_ONLY);
    gpio_set_pull_mode(SD_MMC_CLK, GPIO_PULLUP_ONLY);
    gpio_set_pull_mode(SD_MMC_D0, GPIO_PULLUP_ONLY);

    ret = spi_bus_initialize(host.slot, &bus_cfg, SPI_DMA_CH_AUTO);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to initialize SPI bus: %s", esp_err_to_name(ret));
        
    }

    ret = sdspi_host_init();
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to initialize SDMMC host: %s", esp_err_to_name(ret));
    }

    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
        .format_if_mount_failed = false,
        .max_files = 5,
        .allocation_unit_size = 16 * 1024
    };

    sdmmc_card_t *card;
    ret = esp_vfs_fat_sdspi_mount("/sdcard", &host, &slot_config, &mount_config, &card);
    if (ret != ESP_OK)
    {
        if (ret == ESP_FAIL)
        {
            ESP_LOGE(TAG, "Failed to mount filesystem. If you want the card to be formatted, set format_if_mount_failed = true.");
        }
        else
        {
            ESP_LOGE(TAG, "Failed to initialize the card (%s). Make sure SD card lines have pull-up resistors in place.", esp_err_to_name(ret));
        }
        //return;
    }

    ESP_LOGI(TAG, "SD card mounted successfully");
    sdmmc_card_print_info(stdout, card);

    listFiles("/sdcard", 0);

    ESP_LOGI(TAG, "SD card setup complete");

    return ESP_OK;
}

void listFiles(const char *dirname, int numTabs)
{
    static const char *TAG = "listFiles";

    DIR *dir = opendir(dirname);
    if (!dir)
    {
        ESP_LOGE(TAG, "Failed to open directory: %s", dirname);
        return;
    }
    while (true)
    {
        struct dirent *entry = readdir(dir);
        if (!entry)
        {
            break; // No more files
        }
        for (int i = 0; i < numTabs; i++)
        {
            printf("\t");
        }
        printf("%s", entry->d_name);
        if (entry->d_type == DT_DIR)
        {
            printf("/");
            char path[512]; // Increased buffer size
            snprintf(path, sizeof(path), "%s/%s", dirname, entry->d_name);
            listFiles(path, numTabs + 1);
        }
        else
        {
            // Check if the file has a .jpg or .jpeg extension
            if (strstr(entry->d_name, ".jpg") || strstr(entry->d_name, ".jpeg"))
            {
                printf(" [JPEG File]");
                char path[512]; // Increased buffer size
                snprintf(path, sizeof(path), "%s/%s", dirname, entry->d_name);
                
            }
        }
        printf("\n");
    }
    closedir(dir);
}



