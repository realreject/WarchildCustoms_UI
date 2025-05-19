#include "home_screen.h"
#include "common_ui.h"
#include "event_handler.h"
#include "set_bg.h"

#include "nav_buttons.h"

#define ICON_FONT_SIZE &lv_font_montserrat_48

static const char *TAG = "home_screen.c";

void create_home_screen()
{
      home_screen = lv_obj_create(NULL);
      initialize_global_bg(home_screen);
      attach_bg_to_screen(home_screen);
      create_title(home_screen, "MEGA CAB UI", 0, -135);

      create_horiz_nav_buttons(home_screen); // Create side to side navigation buttons
      create_vert_nav_buttons(home_screen);


      ESP_LOGE(TAG, "home_screen created successfully");
}
