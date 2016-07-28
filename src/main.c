#include <pebble.h>

static Window *s_main_window;
static TextLayer *s_time_layer;  
static GFont s_time_font;
static BitmapLayer *s_background_layer;
static GBitmap *s_background_bitmap;

int numberOfImages = 3;
uint32_t ids[3] = {RESOURCE_ID_ONE_PICTURE, RESOURCE_ID_TWO_PICTURE, RESOURCE_ID_THREE_PICTURE};


static void update_time() {
  //Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);

  //Create a long-lived buffer
  static char buffer[] = "00:00";
  static char minbuffer[] = "00";
  //format the way that the time text is going to be displayed
	strftime(minbuffer, sizeof(minbuffer), "%M", tick_time);
	
	if(tick_time->tm_hour > 12) {
		snprintf(buffer, sizeof(buffer), "%d:%s", (tick_time->tm_hour)-12, minbuffer);
	} else {
		snprintf(buffer, sizeof(buffer), "%d:%s", (tick_time->tm_hour == 0) ? 12 : tick_time->tm_hour, minbuffer);
	}
	

  //Display this time on the TextLayer
  //text_layer_set_text(s_time_layer, buffer);
}

int image = 0;

void select_single_click_handler(ClickRecognizerRef recognizer, void *context) {
  bitmap_layer_set_bitmap(s_background_layer, NULL);
  gbitmap_destroy(s_background_bitmap);
  image++;
  s_background_bitmap = gbitmap_create_with_resource(ids[image % numberOfImages]);
  bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap);
}

void click_config_provider(void *context){
  //window_single_click_subscribe(BUTTON_ID_DOWN, down_single_click_handler);
  //window_single_click_subscribe(BUTTON_ID_UP, up_single_click_handler);
  window_single_click_subscribe(BUTTON_ID_SELECT, select_single_click_handler);
  //window_single_click_subscribe(BUTTON_ID_BACK, back_single_click_handler);
}

static void main_window_load(Window *window) {
  // Create GBitmap, then set to created BitmapLayer
  s_background_bitmap = gbitmap_create_with_resource(ids[image]);
  s_background_layer = bitmap_layer_create(GRect(0, 0, 144, 180));
  bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap);
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_background_layer));
  
  //Create time TextLayer
  s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_ATE_BIT_24));

  //time layer
  s_time_layer = text_layer_create(GRect(0, 0, 0, 0));
  text_layer_set_background_color(s_time_layer, GColorBlack);
  text_layer_set_text_color(s_time_layer, GColorRed);
  text_layer_set_text(s_time_layer, "");

  //Improve the layout to be more like a watchface
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_BITHAM_30_BLACK));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);

  //Add it as a child layer to the Window's root layer
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
	
  window_set_click_config_provider(window, click_config_provider);
}

static void main_window_unload(Window *window) {
  
    bitmap_layer_destroy(s_background_layer);
    
}

// register all events
// static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
// update_time();
 
// }

  


static void init() {
	
  // Register with TickTimerService
//  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  
  // Create main Window element and assign to pointer
  s_main_window = window_create();

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);
  
  // Make sure the time is displayed from the start
  //update_time();
}

static void deinit() {
    // Destroy Window
    window_destroy(s_main_window);
}


//this is the main road map for the app  first it will initialize by running init(), then it will run the app until
//the user exits the app and then will run the deinit() code, which will free up all of the memory that we 
//set aside at the for the app.
int main(void) {
  init();
  app_event_loop();
  deinit();
}