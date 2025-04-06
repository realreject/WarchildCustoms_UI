#ifndef SCREEN4_H
#define SCREEN4_H


void create_screen4(); 
void display_png_image(lv_obj_t *parent, const char *file_path);
void populate_dropdown_with_png_files(lv_obj_t *dropdown);
void dropdown_event_handler(lv_event_t *e);
void display_png_in_window(lv_obj_t *window, const char *file_path);
void to_lowercase(char *str);

#endif // SCREEN4_H