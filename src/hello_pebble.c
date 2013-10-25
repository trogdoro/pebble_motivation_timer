#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"


// Unique ID For App...

#define MY_UUID { 0xFE, 0x52, 0x9C, 0xCE, 0x92, 0xF8, 0x4D, 0x26, 0xB7, 0xB6, 0xBD, 0xCE, 0x37, 0x7B, 0xA8, 0x9C }
PBL_APP_INFO(MY_UUID, "Timez", "The Lord", 1, 0, DEFAULT_MENU_ICON, APP_INFO_STANDARD_APP);

Window window;
/* TextLayer hello_layer; */

TextLayer teaModeLayer;
TextLayer timeLayer; //Timer
TextLayer temperatureLayer;

AppTimerHandle timer_handle;
TextLayer textLayer;



bool timerRunning = false;
bool hidden = false;

short teaMode = 0;

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
  .durations = (uint32_t []) {2, 150, 50, 10},
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

  switch(teaMode)
  {
    case 0:
      vibes_enqueue_custom_pattern(beep_31);
      break;
    case 1:
      vibes_short_pulse();
      break;
    case 2:
      vibes_enqueue_custom_pattern(beep_1);
      break;
    case 3:
      vibes_enqueue_custom_pattern(beep_2);
      break;
    case 4:
      vibes_enqueue_custom_pattern(beep_3);
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



void updateTeaMode()
{

  /*   if(teaMode > 4){ teaMode = 2; } */
  if(teaMode > 4){ teaMode = 0; }

  vibe();

  switch(teaMode)
  {
    case 0:

      /*       if(timerRunning){ vibes_enqueue_custom_pattern(beep_31); } */
      /*       vibe(); */

      text_layer_set_text(&teaModeLayer, "clear mind");
      text_layer_set_text(&temperatureLayer, "");
      /*       text_layer_set_text(&temperatureLayer, "--"); */
      seconds = 45;
      break;
    case 1:
      text_layer_set_text(&teaModeLayer, "think");
      text_layer_set_text(&temperatureLayer, "");
      seconds = 15;
      break;
    case 2:


      // Pull into function!
      // Vibe pattern: ON for 200ms, OFF for 100ms, ON for 400ms:
      /*       static const uint32_t const segments[] = { 100 }; */
      /*       VibePattern pat = { */
      /*         .durations = segments, */
      /*         .num_segments = ARRAY_LENGTH(segments), */
      /*       }; */
      /*       if(timerRunning){ vibes_enqueue_custom_pattern(beep_1); } */
      /*       vibe(); */


      /*       vibes_short_pulse(); */
      text_layer_set_text(&teaModeLayer, "short");
      text_layer_set_text(&temperatureLayer, "");
      seconds = 90;
      break;
    case 3:

      /*       if(timerRunning){ vibes_enqueue_custom_pattern(beep_2); } */
      /*       vibe(); */

      /*       vibes_double_pulse(); */
      text_layer_set_text(&teaModeLayer, "medium");
      text_layer_set_text(&temperatureLayer, "");
      seconds = 150;
      break;
    case 4:
      /*       if(timerRunning){ vibes_enqueue_custom_pattern(beep_3); } */
      /*       vibe(); */

      text_layer_set_text(&teaModeLayer, "long");
      text_layer_set_text(&temperatureLayer, "");
      seconds = 210;
      break;
      /*     case 5: */
      /*       text_layer_set_text(&teaModeLayer, "five"); */
      /*       text_layer_set_text(&temperatureLayer, "--"); */
      /*       seconds = 300; */
      /*       break; */
      /*     case 6: */
      /*       text_layer_set_text(&teaModeLayer, "Oolong"); */
      /*       text_layer_set_text(&temperatureLayer, "195°"); */
      /*       seconds = 180; */
      /*       break; */

      /*     default: */
      /*       teaMode = 2; */
      /*       updateTeaMode(); */
      /*       break; */
  }
  hidden = false;
  layer_set_hidden(&timeLayer.layer, hidden);
  text_layer_set_text(&timeLayer, times[seconds]);
}

void select_double_click_handler(ClickRecognizerRef recognizer, Window *window) {
  /*   timerRunning = !timerRunning; */
  timerRunning = false;
  updateTeaMode();

  // If turned it off, make it beep really quickly
  if(!timerRunning){
    vibes_enqueue_custom_pattern(beep_blip);
  }
}


void up_double_click_handler(ClickRecognizerRef recognizer, Window *window) {
  timerRunning = true;
  /*   teaMode++;   // Cycle through */
  teaMode = 1;
  updateTeaMode();
}

void down_double_click_handler(ClickRecognizerRef recognizer, Window *window) {
  timerRunning = true;   // Make it just turn timer off
  teaMode = 0;
  updateTeaMode();
}



// Long Click handlers...

void up_single_click_handler(ClickRecognizerRef recognizer, Window *window) {

  /*   if(!timerRunning) */
  /*   { */
  timerRunning = true;
  teaMode = 4;
  updateTeaMode();
}

void select_single_click_handler(ClickRecognizerRef recognizer, Window *window) {

  /*   if(!timerRunning) */
  /*   { */
  timerRunning = true;
  teaMode = 3;
  updateTeaMode();
}

void down_single_click_handler(ClickRecognizerRef recognizer, Window *window) {

  /*   text_layer_set_text(&teaModeLayer, "test"); */

  timerRunning = true;
  teaMode = 2;
  updateTeaMode();
}








void config_provider(ClickConfig **config, Window *window) {
  (void)window;

  config[BUTTON_ID_UP]->click.handler = (ClickHandler) up_single_click_handler;
  /*   config[BUTTON_ID_UP]->click.repeat_interval_ms = 100; */
  config[BUTTON_ID_SELECT]->click.handler = (ClickHandler) select_single_click_handler;
  config[BUTTON_ID_DOWN]->click.handler = (ClickHandler) down_single_click_handler;

  config[BUTTON_ID_DOWN]->multi_click.max = 1;

  /*   config[BUTTON_ID_DOWN]->click.repeat_interval_ms = 100; */




  /*   config[BUTTON_ID_UP]->long_click.handler = (ClickHandler) up_long_click_handler; */
  /*   config[BUTTON_ID_UP]->long_click.delay_ms = 300; */
  /*   config[BUTTON_ID_SELECT]->long_click.handler = (ClickHandler) select_long_click_handler; */
  /*   config[BUTTON_ID_SELECT]->long_click.delay_ms = 300; */
  /*   config[BUTTON_ID_DOWN]->long_click.handler = (ClickHandler) down_long_click_handler; */
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





/*
  // multi click config:
  config[BUTTON_ID_SELECT]->multi_click.handler = (ClickHandler) select_multi_click_handler;
  config[BUTTON_ID_SELECT]->multi_click.min = 2;
  config[BUTTON_ID_SELECT]->multi_click.max = 10;
  config[BUTTON_ID_UP]->multi_click.last_click_only = true;

  void select_multi_click_handler(ClickRecognizerRef recognizer, Window *window) {
    ... called for multi-clicks ...
    const uint16_t count = click_number_of_clicks_counted(recognizer);
  }
 */

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
  text_layer_init(&teaModeLayer, GRect(4, 4, 144-4, 40));
  text_layer_set_text_color(&teaModeLayer, GColorWhite);
  text_layer_set_text_alignment(&teaModeLayer, GTextAlignmentCenter);
  text_layer_set_background_color(&teaModeLayer, GColorClear);
  text_layer_set_font(&teaModeLayer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));

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

  updateTeaMode();

  layer_add_child(&window.layer, &timeLayer.layer);
  layer_add_child(&window.layer, &temperatureLayer.layer);
  layer_add_child(&window.layer, &teaModeLayer.layer);

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






/* // When app loads... */

/* void handle_init(AppContextRef ctx) { */

/*   window_init(&window, "Window Name"); */
/*   window_stack_push(&window, true /\* Animated *\/); */

/*   /\*   text_layer_init(&hello_layer, GRect(0, 65, 144, 30)); *\/ */
/*   /\*   text_layer_set_text_alignment(&hello_layer, GTextAlignmentCenter); *\/ */
/*   /\*   text_layer_set_text(&hello_layer, "Hello Again!"); *\/ */
/*   /\*   text_layer_set_font(&hello_layer, fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21)); *\/ */
/*   /\*   layer_add_child(&window.layer, &hello_layer.layer); *\/ */





/* } */


/* void pbl_main(void *params) { */
/*   PebbleAppHandlers handlers = { */
/*     .init_handler = &handle_init */
/*   }; */
/*   app_event_loop(params, &handlers); */
/* } */




/*
/projects/hello_pebble/src/hello_pebble.c
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
  | void updateTeaMode()
 */
