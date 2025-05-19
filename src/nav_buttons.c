#include "nav_buttons.h"
#include "globals.h"
#include "common_ui.h"
#include "event_handler.h"

static const char *TAG = "nav_buttons.c";

// Font & Styling Defines
#define ICON_FONT_SIZE &lv_font_montserrat_48
#define OPA_LEVEL LV_OPA_0
#define SHADOW_OPA LV_OPA_30
#define SHADOW_WIDTH 10

// Dynamic Positioning Defines (based on screen resolution)
#define BTN_WIDTH 40
#define BTN_HEIGHT 230
#define DOWN_BTN_WIDTH 400
#define DOWN_BTN_HEIGHT 40
#define DOWN_BTN_Y_OFFSET 135
#define SIDE_BTN_X_OFFSET ((LV_HOR_RES / 2) - 20)

// Extern the screens that are defined and created elsewhere.
extern lv_obj_t *home_screen;
extern lv_obj_t *bg_sel_screen;
extern lv_obj_t *led_controls_screen;

// Define the horizontal screen array and index.
lv_obj_t *horizontal_screens[NUM_HORIZ_SCREENS];
int current_horizontal_index = 0;

// Define the last press timestamp for navigation buttons.
uint64_t last_press_nav = 0;

void init_horizontal_screens(void)
{
    horizontal_screens[0] = home_screen;
    horizontal_screens[1] = bg_sel_screen;
    horizontal_screens[2] = led_controls_screen;
}

// Navigation: When pressing right, we move to the next screen.
// We'll use MOVE_LEFT animation when going forward.
void go_to_next_screen(lv_event_t *e)
{
    // Check debouncing (use your navigation debounce variable, e.g., last_press_nav)
    // if (!debounce(&last_press_nav, DEBOUNCE_DELAY_MS))
    // return;

    // Increment index with wrap-around.
    current_horizontal_index = (current_horizontal_index + 1) % NUM_HORIZ_SCREENS;
    lv_obj_t *target_screen = horizontal_screens[current_horizontal_index];

    // Attach the global background to the target screen.
    attach_bg_to_screen(target_screen);

    // Animate the transition. For a "next" screen, use MOVE_LEFT.
    lv_scr_load_anim(target_screen, LV_SCR_LOAD_ANIM_MOVE_LEFT, 300, 0, false);
    ESP_LOGI(TAG, "Navigated NEXT to screen index: %d", current_horizontal_index);
}

// Navigation: When pressing left, we move to the previous screen.
// We'll use MOVE_RIGHT animation when moving backward.
void go_to_prev_screen(lv_event_t *e)
{
    if (!debounce(&last_press_nav, DEBOUNCE_DELAY_MS))
        return;

    // Decrement index with wrap-around.
    current_horizontal_index = (current_horizontal_index - 1 + NUM_HORIZ_SCREENS) % NUM_HORIZ_SCREENS;
    lv_obj_t *target_screen = horizontal_screens[current_horizontal_index];

    attach_bg_to_screen(target_screen);

    // For a "previous" navigation, use MOVE_RIGHT animation.
    lv_scr_load_anim(target_screen, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 300, 0, false);
    ESP_LOGI(TAG, "Navigated PREV to screen index: %d", current_horizontal_index);
}

void create_horiz_nav_buttons(lv_obj_t *parent)
{
    /** Right Button **/
    lv_obj_t *right_btn = lv_btn_create(parent);
    lv_obj_set_size(right_btn, BTN_WIDTH, BTN_HEIGHT);
    lv_obj_align(right_btn, LV_ALIGN_CENTER, SIDE_BTN_X_OFFSET, 0);
    lv_obj_set_style_bg_opa(right_btn, OPA_LEVEL, LV_PART_MAIN);
    lv_obj_add_flag(right_btn, LV_OBJ_FLAG_CLICKABLE);

    lv_obj_set_style_shadow_width(right_btn, SHADOW_WIDTH, LV_PART_MAIN);
    lv_obj_set_style_shadow_opa(right_btn, SHADOW_OPA, LV_PART_MAIN);

    lv_obj_set_scrollbar_mode(right_btn, LV_SCROLLBAR_MODE_OFF);
    lv_obj_clear_flag(right_btn, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_add_event_cb(right_btn, button_press_effect, LV_EVENT_PRESSED, NULL);
    lv_obj_add_event_cb(right_btn, go_to_next_screen, LV_EVENT_RELEASED, NULL);

    /** Left Button **/
    lv_obj_t *left_btn = lv_btn_create(parent);
    lv_obj_set_size(left_btn, BTN_WIDTH, BTN_HEIGHT);
    lv_obj_align(left_btn, LV_ALIGN_CENTER, -SIDE_BTN_X_OFFSET, 0);
    lv_obj_set_style_bg_opa(left_btn, OPA_LEVEL, LV_PART_MAIN);
    lv_obj_add_flag(left_btn, LV_OBJ_FLAG_CLICKABLE);

    lv_obj_set_style_shadow_width(left_btn, SHADOW_WIDTH, LV_PART_MAIN);
    lv_obj_set_style_shadow_opa(left_btn, SHADOW_OPA, LV_PART_MAIN);

    lv_obj_set_scrollbar_mode(left_btn, LV_SCROLLBAR_MODE_OFF);
    lv_obj_clear_flag(left_btn, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_add_event_cb(left_btn, button_press_effect, LV_EVENT_PRESSED, NULL);
    lv_obj_add_event_cb(left_btn, go_to_prev_screen, LV_EVENT_RELEASED, NULL);
}

void create_vert_nav_buttons(lv_obj_t *parent)
{
    /** Down Button **/
    lv_obj_t *down_btn = lv_btn_create(parent);
    lv_obj_set_size(down_btn, DOWN_BTN_WIDTH, DOWN_BTN_HEIGHT);
    lv_obj_align(down_btn, LV_ALIGN_CENTER, 0, DOWN_BTN_Y_OFFSET);
    lv_obj_set_style_bg_opa(down_btn, OPA_LEVEL, LV_PART_MAIN);
    lv_obj_add_flag(down_btn, LV_OBJ_FLAG_CLICKABLE);

    lv_obj_set_style_shadow_width(down_btn, SHADOW_WIDTH, LV_PART_MAIN);
    lv_obj_set_style_shadow_opa(down_btn, SHADOW_OPA, LV_PART_MAIN);

    lv_obj_set_scrollbar_mode(down_btn, LV_SCROLLBAR_MODE_OFF);
    lv_obj_clear_flag(down_btn, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_add_event_cb(down_btn, button_press_effect, LV_EVENT_PRESSED, NULL);
    lv_obj_add_event_cb(down_btn, go_to_meter_screen, LV_EVENT_RELEASED, NULL); 
}
