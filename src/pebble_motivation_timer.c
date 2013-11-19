#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"


#define MY_UUID { 0xFE, 0x52, 0x9C, 0xCE, 0x92, 0xF8, 0x4D, 0x26, 0xB7, 0xB6, 0xBD, 0xCE, 0x37, 0x7B, 0xA8, 0x9C }
PBL_APP_INFO(MY_UUID, "Motivation Timer", "Craig Muth", 1, 0, DEFAULT_MENU_ICON, APP_INFO_STANDARD_APP);

Window window;
/* TextLayer hello_layer; */

TextLayer modeLayer;
TextLayer timeLayer; //Timer
TextLayer temperatureLayer;

AppTimerHandle timer_handle;
TextLayer textLayer;



bool timerRunning = false;
bool hidden = false;

short mode = 1;

int seconds = 120;
static char times[600][6];

void reverse(char s[])
{
    int i, j;
    char c;

    for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}
void itoa(int n, char s[])
{
    int i, sign;

    if ((sign = n) < 0)  /* record sign */
        n = -n;          /* make n positive */
    i = 0;
    do {       /* generate digits in reverse order */
        s[i++] = n % 10 + '0';   /* get next digit */
    } while ((n /= 10) > 0);     /* delete it */
    if (sign < 0)
        s[i++] = '-';
    s[i] = '\0';
    reverse(s);
}


void handle_timer(AppContextRef ctx, AppTimerHandle handle, uint32_t cookie) {
  (void)ctx;
  (void)handle;

  hidden = false;
  layer_set_hidden(&timeLayer.layer, hidden);
}


const VibePattern beep_1 = {
  .durations = (uint32_t []) {2, 200, 50, 10},
  .num_segments = 4
};
const VibePattern beep_2 = {
  .durations = (uint32_t []) {2, 150, 50, 150, 50, 10},
  .num_segments = 6
};
const VibePattern beep_3 = {
  .durations = (uint32_t []) {2, 150, 50, 150, 50, 150, 50, 10},
  .num_segments = 8
};

const VibePattern beep_31 = {
  .durations = (uint32_t []) {2, 150, 150, 150, 50, 10},
  .num_segments = 6
};

const VibePattern beep_blip = {
  /*   .durations = (uint32_t []) {2, 10, 30, 60, 30, 10}, */
  .durations = (uint32_t []) {2, 150, 30, 10},
  .num_segments = 4
};


// Called once per second
void vibe() {

  if(! timerRunning){ return; }

  // Beep based on mode...

  switch(mode)
    {
    case 0:
      vibes_enqueue_custom_pattern(beep_31);
      break;
    case 1:   // Short
      vibes_enqueue_custom_pattern(beep_1);
      break;
    case 2:   // Medium
      vibes_enqueue_custom_pattern(beep_2);
      break;
    case 3:   // Long
      vibes_enqueue_custom_pattern(beep_3);
      break;
    case 4:
      vibes_short_pulse();
      break;

    default:
      vibes_short_pulse();
      break;
  }


}

void handle_second_tick(AppContextRef ctx, PebbleTickEvent *t)
{

  (void)ctx;
  (void)t;

  if (timerRunning)
  {
    seconds--;
    text_layer_set_text(&timeLayer, times[seconds]);
  }

  if (seconds <= 0)
  {
    vibe();

      /*     vibes_double_pulse(); */
    seconds = 0;
    text_layer_set_text(&timeLayer, times[seconds]);
    hidden = true;
    layer_set_hidden(&timeLayer.layer, hidden);
    timer_handle = app_timer_send_event(ctx, 500 , 1);
  }

}



void updateMode()
{

  /*   if(mode > 4){ mode = 2; } */
  if(mode > 4){ mode = 0; }

  vibe();

  // Reset everything based on mode...

  switch(mode)
  {
    case 0:
      text_layer_set_text(&modeLayer, "very short");
      seconds = 15;
      break;
    case 1:
      text_layer_set_text(&modeLayer, "short");
      seconds = 45;
      break;
    case 2:
      text_layer_set_text(&modeLayer, "medium");
      seconds = 90;
      break;
    case 3:
      text_layer_set_text(&modeLayer, "long");
      seconds = 150;
      break;
    case 4:
      text_layer_set_text(&modeLayer, "very long");
      seconds = 210;
      break;
  }

  text_layer_set_text(&temperatureLayer, "");

  hidden = false;
  layer_set_hidden(&timeLayer.layer, hidden);
  text_layer_set_text(&timeLayer, times[seconds]);
}

void select_double_click_handler(ClickRecognizerRef recognizer, Window *window) {
  /*   timerRunning = !timerRunning; */
  timerRunning = false;
  updateMode();

  // If turned it off, make it beep really quickly
  if(!timerRunning){
    vibes_enqueue_custom_pattern(beep_blip);
  }
}


void up_double_click_handler(ClickRecognizerRef recognizer, Window *window) {
  timerRunning = true;
  /*   mode++;   // Cycle through */
  mode = 4;
  updateMode();
}

void down_double_click_handler(ClickRecognizerRef recognizer, Window *window) {
  timerRunning = true;   // Make it just turn timer off
  mode = 0;
  updateMode();
}


// Long Click handlers...

void up_single_click_handler(ClickRecognizerRef recognizer, Window *window) {

  /*   if(!timerRunning) */
  /*   { */
  timerRunning = true;
  mode = 3;
  updateMode();
}

void select_single_click_handler(ClickRecognizerRef recognizer, Window *window) {

  /*   if(!timerRunning) */
  /*   { */
  timerRunning = true;
  mode = 2;
  updateMode();
}

void down_single_click_handler(ClickRecognizerRef recognizer, Window *window) {

  /*   text_layer_set_text(&modeLayer, "test"); */

  timerRunning = true;
  mode = 1;
  updateMode();
}








void config_provider(ClickConfig **config, Window *window) {
  (void)window;

  // Map Buttons to Methods...

  config[BUTTON_ID_UP]->click.handler = (ClickHandler) up_single_click_handler;
  /*   config[BUTTON_ID_UP]->click.repeat_interval_ms = 100; */
  config[BUTTON_ID_SELECT]->click.handler = (ClickHandler) select_single_click_handler;
  config[BUTTON_ID_DOWN]->click.handler = (ClickHandler) down_single_click_handler;

  config[BUTTON_ID_DOWN]->multi_click.max = 1;

  /*   config[BUTTON_ID_DOWN]->click.repeat_interval_ms = 100; */




  config[BUTTON_ID_UP]->long_click.handler = (ClickHandler) select_double_click_handler;
  config[BUTTON_ID_SELECT]->long_click.handler = (ClickHandler) select_double_click_handler;
  config[BUTTON_ID_DOWN]->long_click.handler = (ClickHandler) select_double_click_handler;
  /*   config[BUTTON_ID_DOWN]->long_click.delay_ms = 300; */


  config[BUTTON_ID_UP]->multi_click.handler = (ClickHandler) up_double_click_handler;
  config[BUTTON_ID_UP]->multi_click.min = 2;
  config[BUTTON_ID_UP]->multi_click.max = 2;

  config[BUTTON_ID_SELECT]->multi_click.handler = (ClickHandler) select_double_click_handler;
  config[BUTTON_ID_SELECT]->multi_click.min = 2;
  config[BUTTON_ID_SELECT]->multi_click.max = 2;

  config[BUTTON_ID_DOWN]->multi_click.handler = (ClickHandler) down_double_click_handler;
  config[BUTTON_ID_DOWN]->multi_click.min = 2;
  config[BUTTON_ID_DOWN]->multi_click.max = 2;

}

void populateTimeArray()
{
  int r = 0;
  int h = 0;
  for(int i = 0; i < 600; i++)
  {
    r = i%60;
    h = (int) i/60;
    if ( i > 60)
      itoa(h,(char*)&times[i][0]);
    else
      times[i][0] = '0';


    times[i][1] = ':';


    if (r < 10)
    {
      times[i][2] = '0';
      itoa(r,(char*)&times[i][3]);
    }
    else
    {
      itoa(r,(char*)&times[i][2] );
    }
  }
}

// Handle the start-up of the app
void handle_init(AppContextRef app_ctx) {
  populateTimeArray();

  // Create our app's base window
  window_init(&window, "Tea Timer");
  window_stack_push(&window, true /* Animated */);
  window_set_background_color(&window, GColorBlack);


  resource_init_current_app(&VERSION);

  // Init the text layer used to show the Temperature
  text_layer_init(&temperatureLayer, GRect(4, 104, 144-4, 40));
  text_layer_set_text_color(&temperatureLayer, GColorWhite);
  text_layer_set_text_alignment(&temperatureLayer, GTextAlignmentCenter);
  text_layer_set_background_color(&temperatureLayer, GColorClear);
  text_layer_set_font(&temperatureLayer, fonts_get_system_font(FONT_KEY_BITHAM_30_BLACK));

  // Init the text layer used to show the Tea Mode
  text_layer_init(&modeLayer, GRect(4, 4, 144-4, 40));
  text_layer_set_text_color(&modeLayer, GColorWhite);
  text_layer_set_text_alignment(&modeLayer, GTextAlignmentCenter);
  text_layer_set_background_color(&modeLayer, GColorClear);
  text_layer_set_font(&modeLayer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));

  // Init the text layer used to show the time left
  text_layer_init(&timeLayer, GRect(4, 44, 144-4, 60));
  text_layer_set_text_color(&timeLayer, GColorWhite);
  text_layer_set_text_alignment(&timeLayer, GTextAlignmentCenter);
  text_layer_set_background_color(&timeLayer, GColorClear);
  /*   text_layer_set_font(&timeLayer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD)); */
  text_layer_set_font(&timeLayer, fonts_get_system_font(FONT_KEY_ROBOTO_BOLD_SUBSET_49));
  // Ensures time is displayed immediately (will break if NULL tick event accessed).
  // (This is why it's a good idea to have a separate routine to do the update itself.)
  handle_second_tick(app_ctx, NULL);

  updateMode();

  layer_add_child(&window.layer, &timeLayer.layer);
  layer_add_child(&window.layer, &temperatureLayer.layer);
  layer_add_child(&window.layer, &modeLayer.layer);

  // Attach our desired button functionality
  window_set_click_config_provider(&window, (ClickConfigProvider) config_provider);
}

void pbl_main(void *params) {
  PebbleAppHandlers handlers = {

    // Handle app start
    .init_handler = &handle_init,
      .timer_handler = &handle_timer,

    // Handle time updates
    .tick_info = {
      .tick_handler = &handle_second_tick,
      .tick_units = SECOND_UNIT
    }

  };
  app_event_loop(params, &handlers);
}


/*
/projects/pebble_motivation_timer/src/pebble_motivation_timer.c
  > Click Handlers
  single
  | void up_single_click_handler
  | void select_single_click_handler
  | void down_single_click_handler
  double
  | void up_double_click_handler
  | void select_double_click_handler
  | void down_double_click_handler

  > Called when buttons clicked
  | void updateMode()
 */
