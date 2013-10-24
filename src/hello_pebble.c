#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"


// Unique ID For App...

#define MY_UUID { 0xFE, 0x52, 0x9C, 0xCE, 0x92, 0xF8, 0x4D, 0x26, 0xB7, 0xB6, 0xBD, 0xCE, 0x37, 0x7B, 0xA8, 0x9C }
PBL_APP_INFO(MY_UUID,
             "Timez", "The Lord",
             1, 0, /* App version */
             DEFAULT_MENU_ICON,
             APP_INFO_STANDARD_APP);

Window window;
TextLayer hello_layer;


// When app loads...

void handle_init(AppContextRef ctx) {

  window_init(&window, "Window Name");
  window_stack_push(&window, true /* Animated */);

  text_layer_init(&hello_layer, GRect(0, 65, 144, 30));
  text_layer_set_text_alignment(&hello_layer, GTextAlignmentCenter);
  text_layer_set_text(&hello_layer, "Hello Again!");
  text_layer_set_font(&hello_layer, fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21));
  layer_add_child(&window.layer, &hello_layer.layer);





}


void pbl_main(void *params) {
  PebbleAppHandlers handlers = {
    .init_handler = &handle_init
  };
  app_event_loop(params, &handlers);
}
