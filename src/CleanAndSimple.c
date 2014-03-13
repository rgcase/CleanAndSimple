#include <pebble.h>

Window *window;
TextLayer *text_layer;
TextLayer *text_day_layer;
TextLayer *text_date_layer;
char buffer[] = "00:00";
char day[] = "Xxxxxxxxx";
char date[] = "Xxxxxxxxx 00";
InverterLayer *inv_layer;

void tick_handler(struct tm *tick_time, TimeUnits units_changed);
void window_load(Window *window);
void window_unload(Window *window);
void init();
void deinit();

int main(void) {
  init();
  app_event_loop();
  deinit();
}

void init() {
  window = window_create();
  window_set_window_handlers(window, (WindowHandlers) {
      .load = window_load,
      .unload = window_unload,
      });
  window_stack_push(window, true);
  tick_timer_service_subscribe(MINUTE_UNIT, (TickHandler) tick_handler);
}

void deinit() {
  //window_destroy(window);
}

void window_load(Window *window) {
  text_layer = text_layer_create(GRect(0, 53, 144, 168));
  text_layer_set_background_color(text_layer, GColorClear);
  text_layer_set_text_color(text_layer, GColorBlack);
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  text_layer_set_font(text_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));

  layer_add_child(window_get_root_layer(window), (Layer*) text_layer);
  
  text_layer_set_text(text_layer, "CleanAndSimple");

  inv_layer = inverter_layer_create(GRect(0,50,144,62));
  layer_add_child(window_get_root_layer(window), (Layer*) inv_layer);
  
  text_day_layer = text_layer_create(GRect(0,10,144,50));
  text_layer_set_background_color(text_day_layer, GColorClear);
  text_layer_set_text_color(text_day_layer, GColorBlack);
  text_layer_set_text_alignment(text_day_layer, GTextAlignmentLeft);
  text_layer_set_font(text_day_layer, fonts_get_system_font(
    FONT_KEY_GOTHIC_28_BOLD));
  layer_add_child(window_get_root_layer(window), (Layer*) text_day_layer);
    
  text_date_layer = text_layer_create(GRect(0,112,144,168 - 112));
  text_layer_set_background_color(text_date_layer, GColorClear);
  text_layer_set_text_color(text_date_layer, GColorBlack);
  text_layer_set_text_alignment(text_date_layer, GTextAlignmentRight);
  text_layer_set_font(text_date_layer, fonts_get_system_font(
    FONT_KEY_GOTHIC_28_BOLD));
  layer_add_child(window_get_root_layer(window), (Layer*) text_date_layer);

  struct tm *t;
  time_t temp;
  temp = time(NULL);
  t = localtime(&temp);

  tick_handler(t, MINUTE_UNIT);
}

void window_unload(Window *window) {
  tick_timer_service_unsubscribe();
  text_layer_destroy(text_layer);
  inverter_layer_destroy(inv_layer);
  window_destroy(window);
}

void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  char* time_format;
  
  strftime(day, sizeof(day), "%A", tick_time);
  text_layer_set_text(text_day_layer, day);
  
  strftime(date, sizeof(date), "%B %e", tick_time);
  text_layer_set_text(text_date_layer, date);
  
  if (clock_is_24h_style()) {
    time_format = "%R";
  } else {
    time_format = "%I:%M";
  }
  
  strftime(buffer, sizeof(buffer), time_format, tick_time);
  text_layer_set_text(text_layer, buffer);
}
