#include "bg_sel_screen.h"
#include "set_bg.h"
#include "common_ui.h"
#include "nav_buttons.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

// ESP error logging tag
static const char *TAG = "bg_sel_screen.c";

void create_bg_sel_screen()
{
    bg_sel_screen = lv_obj_create(NULL); // Create a new screen object 
    initialize_global_bg(bg_sel_screen);
    create_title(bg_sel_screen, "CUSTOMIZE BACKGROUND IMAGE", 0, -135); // Create the title using the common function  
    create_horiz_nav_buttons(bg_sel_screen); // Create side to side navigation buttons  

    // Create a window for displaying the image
    image_window = lv_obj_create(bg_sel_screen);
    lv_obj_set_size(image_window, 260, 160);            // Set the window size
    lv_obj_align(image_window, LV_ALIGN_CENTER, 0, 40); // Position below the dropdown
    lv_obj_set_style_border_color(image_window, lv_palette_main(LV_PALETTE_BLUE), 0);
    lv_obj_set_style_bg_opa(image_window, 0, 0);       // Set background opacity to 0 (transparent)
    lv_obj_set_style_border_width(image_window, 0, 0); // Set border width to 0 (no border)
    lv_obj_set_style_pad_all(image_window, 0, 0);
    lv_obj_set_scrollbar_mode(image_window, LV_SCROLLBAR_MODE_OFF);
    lv_obj_clear_flag(image_window, LV_OBJ_FLAG_SCROLLABLE);

    // Create the dropdown menu
    lv_obj_t *dropdown = lv_dropdown_create(bg_sel_screen);
    lv_obj_set_width(dropdown, 300);                 // Set dropdown width
    lv_obj_align(dropdown, LV_ALIGN_CENTER, 0, -80); // Align the dropdown

    // Populate the dropdown with PNG files from "S:/png/"
    populate_dropdown_with_png_files(dropdown);

    ESP_LOGE(TAG, "finished populating dropdown with PNG files");

    // Attach event callback to handle user selection
    lv_obj_add_event_cb(dropdown, dropdown_event_handler, LV_EVENT_VALUE_CHANGED, NULL);

    ESP_LOGE(TAG, "bg_sel_screen created successfully");
}

void populate_dropdown_with_png_files(lv_obj_t *dropdown)
{
    lv_fs_dir_t dir;
    lv_fs_res_t res = lv_fs_dir_open(&dir, "S:/png/");
    if (res != LV_FS_RES_OK)
    {
        ESP_LOGE(TAG, "Failed to open directory 'S:/png/'. Error code: %d", res);
        lv_dropdown_set_options(dropdown, "Error: Directory Not Found\n");
        return;
    }

    char file_name[256];
    char options[1024] = "";
    ESP_LOGI(TAG, "Directory opened successfully. Reading files...");

    while (lv_fs_dir_read(&dir, file_name) == LV_FS_RES_OK)
    {
        if (strlen(file_name) == 0)
        {
            ESP_LOGI(TAG, "Reached end of directory.");
            break;
        }

        ESP_LOGI(TAG, "File found: %s", file_name);

        // Case-insensitive check for .png
        size_t len = strlen(file_name);
        if (len > 4 && (strcasecmp(file_name + len - 4, ".png") == 0))
        {
            if ((strlen(options) + len + 2) < sizeof(options))
            { // Prevent buffer overflow
                strcat(options, file_name);
                strcat(options, "\n");
                ESP_LOGI(TAG, "PNG file added: %s", file_name);
            }
            else
            {
                ESP_LOGW(TAG, "Buffer full. Skipping file: %s", file_name);
                break;
            }
        }
        else
        {
            ESP_LOGI(TAG, "Skipped non-PNG file: %s", file_name);
        }
    }

    if (strlen(options) == 0)
    {
        ESP_LOGW(TAG, "No PNG files found.");
        lv_dropdown_set_options(dropdown, "No Files Found\n");
    }
    else
    {
        ESP_LOGI(TAG, "Dropdown options:\n%s", options);
        lv_dropdown_set_options(dropdown, options);
    }

    lv_fs_dir_close(&dir);
}

void to_lowercase(char *str)
{
    while (*str)
    {
        *str = tolower(*str);
        str++;
    }
}

void dropdown_event_handler(lv_event_t *e)
{   
    ESP_LOGI(TAG, "Dropdown event handler triggered.");
        
    lv_obj_t *dropdown = lv_event_get_target(e);

    // Get the selected file name
    char selected_file[256];
    lv_dropdown_get_selected_str(dropdown, selected_file, sizeof(selected_file));

    // Convert the file name to lowercase
    to_lowercase(selected_file);

    // Construct the full file path
    char file_path[512];
    snprintf(file_path, sizeof(file_path), "S:/png/%s", selected_file);

    ESP_LOGI(TAG, "Selected file path: %s", file_path);

    // Save the selected file path to NVS
    if (save_background_to_nvs(file_path))
    {
        ESP_LOGI(TAG, "File path saved to NVS successfully.");
    }
    else
    {
        ESP_LOGE(TAG, "Failed to save file path to NVS.");
    }
    
    // Display the image in the dedicated window
    //display_png_in_window(image_window, file_path);
    
    
    update_global_bg(file_path); // Update the global background object  
}

void display_png_in_window(lv_obj_t *window, const char *file_path)
{
    ESP_LOGI(TAG, "Displaying PNG image in window: %s", file_path);

    // Delete any previously displayed image, if necessary
    static lv_obj_t *prev_img = NULL;
    if (prev_img != NULL)
    {
        lv_obj_del(prev_img);
        prev_img = NULL;
    }

    // Create the image object
    lv_obj_t *img = lv_img_create(window);
    if (img == NULL)
    {
        ESP_LOGE(TAG, "Failed to create image object.");
        return;
    }

    // Set the PNG file as the source
    lv_img_set_src(img, file_path);
    if (lv_img_get_src(img) == NULL)
    {
        ESP_LOGE(TAG, "Failed to set image source.");
        lv_obj_del(img);
        return;
    }

    // Get the image size and container size
    // lv_coord_t img_w = lv_obj_get_width(img);
    // lv_coord_t img_h = lv_obj_get_height(img);
    lv_coord_t img_w = 480;
    lv_coord_t img_h = 320;
    lv_coord_t win_w = lv_obj_get_width(window);
    lv_coord_t win_h = lv_obj_get_height(window);

    // Sanity check for zero dimensions
    if (img_w == 0 || img_h == 0)
    {
        ESP_LOGE(TAG, "Invalid image dimensions: img_w=%d, img_h=%d", img_w, img_h);
        lv_obj_del(img);
        return;
    }

    // Calculate scaling factor to maintain aspect ratio
    lv_coord_t zoom_factor;
    if (img_w * win_h > win_w * img_h)
    {
        zoom_factor = (win_w * 256) / img_w; // Scale based on width
    }
    else
    {
        zoom_factor = (win_h * 256) / img_h; // Scale based on height
    }
    lv_img_set_zoom(img, zoom_factor);

    // Align the image to the center of the window
    lv_obj_align(img, LV_ALIGN_CENTER, 0, 0);
    ESP_LOGI(TAG, "Image displayed in window with zoom factor: %d", zoom_factor);

    // Store the reference to the displayed image
    prev_img = img;
}