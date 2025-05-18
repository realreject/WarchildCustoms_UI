#include "set_bg.h"
#include "event_handler.h"
#include "viking_bg_480_320.h"
#include <esp_log.h>

#define STORAGE_NAMESPACE "storage"
static const char *TAG = "set_bg.c";

#define MAX_FILE_PATH_LENGTH 256

/* void initialize_global_bg(lv_obj_t *parent)
{
    if (global_bg_img == NULL)
    {
        global_bg_img = lv_img_create(parent);
        lv_img_set_src(global_bg_img, &viking_bg_480_320);  // Use the image directly from the C file
        lv_obj_align(global_bg_img, LV_ALIGN_CENTER, 0, 0); // Center alignment
    }

} */

void initialize_global_bg(lv_obj_t *parent)
{
    // Check if the global background image object already exists
    if (global_bg_img == NULL)
    {
        ESP_LOGI(TAG, "Initializing global background image...");

        // Create the global background image object
        global_bg_img = lv_img_create(parent);
        lv_obj_align(global_bg_img, LV_ALIGN_CENTER, 0, 0); // Center alignment

        // Attempt to fetch the background file path from NVS
        char file_path[MAX_FILE_PATH_LENGTH];
        if (nvs_get_background_file(file_path, sizeof(file_path)) && is_file_valid(file_path))
        {
            ESP_LOGI(TAG, "Valid background file found and accessible: %s", file_path);
            set_png_image(file_path); // Set the file as the background
        }
        else
        {
            ESP_LOGW(TAG, "No valid background file in NVS. Using default image.");
            lv_img_set_src(global_bg_img, &viking_bg_480_320); // Fallback to the default image
        }
    }
    else
    {
        ESP_LOGI(TAG, "Global background image already initialized. Skipping reinitialization.");
    }
}

bool nvs_get_background_file(char *file_path, size_t max_size)
{

    nvs_handle_t nvs_handle;
    esp_err_t err = nvs_open(STORAGE_NAMESPACE, NVS_READONLY, &nvs_handle);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to open NVS. Error: %d", err);
        return false;
    }

    err = nvs_get_str(nvs_handle, "background_file", file_path, &max_size);

    nvs_close(nvs_handle);

    if (err == ESP_OK)
    {
        ESP_LOGI("NVS", "Retrieved background file path: %s", file_path);
        return true;
    }
    else if (err == ESP_ERR_NVS_NOT_FOUND)
    {
        ESP_LOGW("NVS", "No background file found in NVS.");
    }
    else
    {
        ESP_LOGE(TAG, "Failed to retrieve background file. Error: %d", err);
    }

    return false;
}

void attach_bg_to_screen(lv_obj_t *screen)
{
    ESP_LOGE(TAG, "Attaching BG to SCREEN");

    // Ensure screen has a black background before attaching the global_bg_img
    lv_obj_set_style_bg_color(screen, lv_color_black(), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(screen, LV_OPA_COVER, LV_PART_MAIN); // Ensure full opacity

    if (global_bg_img == NULL)
    {
        ESP_LOGE(TAG, "Global background image is not initialized.");
        return;
    }

    // Reparent the background to the given screen
    lv_obj_set_parent(global_bg_img, screen);

    lv_obj_move_background(global_bg_img); // Ensure it stays at the back

    // Refresh rendering if needed
    lv_obj_invalidate(global_bg_img);
}

void update_global_bg(const char *file_path)
{
    ESP_LOGI(TAG, "Updating global background with file: %s", file_path);

    if (!is_file_valid(file_path))
    {
        ESP_LOGE(TAG, "Invalid file path: %s", file_path);
        return;
    }

    ESP_LOGI("update_global_bg function", "passed file path: %s", file_path);

    // Use set_png_image directly to update the background image
    set_png_image(file_path);

    lv_obj_move_background(global_bg_img); // Ensure it stays at the back

    // Refresh rendering if needed
    lv_obj_invalidate(global_bg_img);
}

void set_png_image(const char *file_path)
{
    ESP_LOGI("set_png_image function", "passed file path: %s", file_path);

    // Check if the global background image object exists
    if (global_bg_img == NULL)
    {
        ESP_LOGE(TAG, "Global background image is not initialized.");
        return;
    }

    // Set the PNG file as the source for the global background
    lv_img_set_src(global_bg_img, file_path);
    if (lv_img_get_src(global_bg_img) == NULL)
    {
        ESP_LOGE(TAG, "Failed to set background image source: %s", file_path);
        return;
    }

    // Ensure the image is sized and positioned correctly
    lv_obj_set_size(global_bg_img, LV_HOR_RES, LV_VER_RES); // Match screen resolution
    lv_obj_align(global_bg_img, LV_ALIGN_CENTER, 0, 0);     // Align to center

    ESP_LOGI(TAG, "Global background image updated successfully: %s", file_path);
}

bool is_file_valid(const char *file_path)
{
    lv_fs_file_t file;
    lv_fs_res_t res = lv_fs_open(&file, file_path, LV_FS_MODE_RD);
    if (res == LV_FS_RES_OK)
    {
        lv_fs_close(&file);
        ESP_LOGI(TAG, "File exists and is accessible: %s", file_path);
        return true;
    }
    else
    {
        ESP_LOGE(TAG, "File does not exist or cannot be opened: %s", file_path);
        return false;
    }
}

// called from bg_sel_screen.c void dropdown_event_handler(lv_event_t *e) function
bool save_background_to_nvs(const char *file_path)
{
    nvs_handle_t nvs_handle;
    esp_err_t err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &nvs_handle); // Open NVS with write access
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to open NVS. Error: %d", err);
        return false; // Fix: Return false on failure
    }

    // Save the file path to NVS
    err = nvs_set_str(nvs_handle, "background_file", file_path);
    if (err == ESP_OK)
    {
        ESP_LOGI(TAG, "Background file path saved: %s", file_path);
    }
    else
    {
        ESP_LOGE(TAG, "Failed to save background file to NVS. Error: %d", err);
        nvs_close(nvs_handle);
        return false; // Fix: Return false if save fails
    }

    // Commit changes
    err = nvs_commit(nvs_handle);
    nvs_close(nvs_handle); // Close the NVS handle
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to commit changes to NVS. Error: %d", err);
        return false; // Fix: Return false if commit fails
    }

    return true; // Fix: Return true on success
}

// not sure i am calling this anywhere
bool validate_image_with_decoder(const char *file_path)
{
    lv_img_header_t header; // Image header for metadata
    if (lv_img_decoder_get_info(file_path, &header) == LV_RES_OK)
    {
        ESP_LOGI(TAG, "Valid image file detected: %s", file_path);
        return true;
    }
    else
    {
        ESP_LOGE(TAG, "Failed to decode image file: %s", file_path);
        return false;
    }
}
