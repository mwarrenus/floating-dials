#include <pebble.h>

static Window *window;
static Layer *minute_dial, *minute_hand, *hour_dial, *hour_hand, *second_dial, *second_hand;

#define TWENTYFOUR true

#ifndef TWENTYFOUR
//#define MY_UUID { 0xA7, 0x13, 0xC7, 0x01, 0x7B, 0xB1, 0x4E, 0x19, 0x9B, 0xEF, 0x5A, 0xF2, 0x15, 0xD6, 0xFC, 0x51 }
//#define APP_NAME "Floating Dials"
#define MAX_HOURS 12
#define MAX_HOURS_LABEL 12
#define HOUR_DIVISOR 2
#else
//#define MY_UUID { 0xA7, 0x13, 0xC7, 0x01, 0x7B, 0xB1, 0x4E, 0x19, 0x9B, 0xEF, 0x5A, 0xF2, 0x15, 0xD6, 0xFC, 0x52 }
//#define APP_NAME "Floating Dials (24h)"
#define MAX_HOURS 24
#define MAX_HOURS_LABEL 0
#define HOUR_DIVISOR 1
#define HOUR_COUNTER 3
#endif

#ifndef INVERTED
#define BACKGROUND GColorBlack
#define FOREGROUND GColorWhite
#else
#define BACKGROUND GColorWhite
#define FOREGROUND GColorBlack
#endif


GPoint rotate_point(int angle, int max, int radius, GPoint center) {
  GPoint point;
  int32_t second_angle = TRIG_MAX_ANGLE * angle / max;
  point.y = (-cos_lookup(second_angle) * radius / TRIG_MAX_RATIO) + center.y;
  point.x = (sin_lookup(second_angle) * radius / TRIG_MAX_RATIO) + center.x;
  return point;
}

#define MINUTE_RADIUS 71
#define FIFTEEN_MARK_LENGTH 5
#define FIVE_MARK_LENGTH 2

#define HOUR_RADIUS (MINUTE_RADIUS / 2) 
#define THREE_MARK_LENGTH FIFTEEN_MARK_LENGTH
#define ONE_MARK_LENGTH FIVE_MARK_LENGTH
#define HOUR_BORDER 1
#define DIVISOR_MARK_LENGTH 0
#define HOUR_MARKS 4

void hour_dial_update(Layer *layer, GContext* gctx) {
  static char hour_text[] = "12";
  GRect frame=layer_get_frame(layer);
  GPoint center = grect_center_point(&frame);
  graphics_context_set_fill_color(gctx, BACKGROUND);
  int32_t radius = HOUR_RADIUS ;
  graphics_fill_circle(gctx, center, radius);
  radius -= HOUR_BORDER;
  graphics_context_set_stroke_color(gctx, FOREGROUND);
  graphics_context_set_text_color(gctx, FOREGROUND);
  for (int angle=0; angle < MAX_HOURS * HOUR_DIVISOR; angle++){
    int32_t length;
    if(angle % ((MAX_HOURS / HOUR_MARKS) * HOUR_DIVISOR) == 0) {
      length = THREE_MARK_LENGTH;
    } else
#ifndef TWENTYFOUR 
      if(angle % HOUR_DIVISOR == 0) {
#else
      if(angle % HOUR_COUNTER == 0) {
#endif
      length = ONE_MARK_LENGTH;
    } else {
      length = DIVISOR_MARK_LENGTH;
    }
    GPoint outer=rotate_point(angle, MAX_HOURS * HOUR_DIVISOR, radius, center);
    GPoint inner=rotate_point(angle, MAX_HOURS * HOUR_DIVISOR, radius - length, center);
    graphics_draw_line(gctx, outer, inner);

    if (angle % (((MAX_HOURS / HOUR_MARKS)) * HOUR_DIVISOR) == 0) {
      GPoint text_center=rotate_point(angle, MAX_HOURS * HOUR_DIVISOR, radius - THREE_MARK_LENGTH - 10 , center);
      snprintf(hour_text, sizeof(hour_text), "%2d", angle?angle / HOUR_DIVISOR:MAX_HOURS_LABEL);
      graphics_draw_text(gctx, hour_text, fonts_get_system_font(FONT_KEY_GOTHIC_14),
			 GRect(text_center.x - 10, text_center.y - 9, 20, 14),
			 GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);    
    }
  }
}


void minute_dial_update(Layer *layer, GContext* gctx) {
  static char minute_text[] = "59";
  graphics_context_set_stroke_color(gctx, FOREGROUND);
  graphics_context_set_text_color(gctx, FOREGROUND);
  GRect frame=layer_get_frame(layer);
  GPoint center = grect_center_point(&frame);
  for (int angle=0; angle < 60; angle++){
    int32_t radius = MINUTE_RADIUS ;
    int32_t length;
    if(angle % 15 == 0){
      length = FIFTEEN_MARK_LENGTH;
    } else if(angle % 5 == 0){
      length=2;
    } else{
      length=0;
    }
    GPoint outer=rotate_point(angle, 60, radius, center);
    GPoint inner=rotate_point(angle, 60, radius - length, center);
    graphics_draw_line (gctx, outer, inner);

    if(angle % 5 == 0) {
      GPoint text_center=rotate_point(angle, 60, radius - FIFTEEN_MARK_LENGTH - 10 , center);
      snprintf(minute_text, sizeof(minute_text), "%02d", angle);
      graphics_draw_text(gctx, minute_text, fonts_get_system_font(FONT_KEY_GOTHIC_14),
			 GRect(text_center.x - 10, text_center.y - 9, 20, 14),
			 GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);    
    }
  }
}

static GPath *minute_hand_path, *hour_hand_path;

void minute_hand_update(Layer *layer, GContext* gctx) {
  graphics_context_set_fill_color(gctx, FOREGROUND);
  graphics_context_set_stroke_color(gctx, BACKGROUND);
  //graphics_context_set_fill_color(gctx, BACKGROUND);
  //graphics_context_set_stroke_color(gctx, FOREGROUND);

  time_t now = time(NULL);
  struct tm *time = localtime(&now);
#ifdef TEST
  gpath_rotate_to(minute_hand_path, TRIG_MAX_ANGLE * time->tm_sec / 60);
  GPoint hour=rotate_point(time->tm_sec, 60, HOUR_RADIUS, GPoint (0, 0));
#else
  gpath_rotate_to(minute_hand_path, TRIG_MAX_ANGLE * time->tm_min / 60);
  GPoint hour=rotate_point(time->tm_min, 60, HOUR_RADIUS, GPoint (0, 0));
#endif
  gpath_draw_filled(gctx, minute_hand_path);
  gpath_draw_outline(gctx, minute_hand_path);

  layer_set_bounds(hour_dial,GRect (-hour.x, -hour.y, 144, 168));	

}

void hour_hand_update(Layer *layer, GContext* gctx) {
  graphics_context_set_fill_color(gctx, FOREGROUND);
  graphics_context_set_stroke_color(gctx, BACKGROUND);
  //graphics_context_set_fill_color(gctx, BACKGROUND);
  //graphics_context_set_stroke_color(gctx, FOREGROUND);
  time_t now = time(NULL);
  struct tm *time = localtime(&now);
#ifdef TEST
  gpath_rotate_to(hour_hand_path, TRIG_MAX_ANGLE * time->tm_sec / 60);
#else
  //  gpath_rotate_to(hour_hand_path, TRIG_MAX_ANGLE * ( (time->tm_hour + (time->tm_min / 60)) / 12 ) );
  gpath_rotate_to(hour_hand_path, (TRIG_MAX_ANGLE * (((time->tm_hour % MAX_HOURS) * 6) + (time->tm_min / 10))) / (MAX_HOURS * 6));
#endif
  gpath_draw_filled(gctx, hour_hand_path);
  gpath_draw_outline(gctx, hour_hand_path);
}

static const GPathInfo minute_hand_points = {
  3,
  (GPoint []) {
    { -(FIFTEEN_MARK_LENGTH - MINUTE_RADIUS) / 4, 0 },
    { (FIFTEEN_MARK_LENGTH - MINUTE_RADIUS) / 4, 0 },
    { 0, (FIFTEEN_MARK_LENGTH - MINUTE_RADIUS) }
  }
};
static const GPathInfo hour_hand_points = {
  3,
  (GPoint []) {
    { -(THREE_MARK_LENGTH - HOUR_RADIUS)/4, 0 },
    { (THREE_MARK_LENGTH - HOUR_RADIUS)/4, 0 },
    { 0, (THREE_MARK_LENGTH - HOUR_RADIUS) }
  }
};

static void handle_tick(struct tm *tick_time, TimeUnits units_changed) {
  layer_mark_dirty(minute_hand);
  layer_mark_dirty(hour_hand);
}

static void window_load(Window *window) {
  // create all layers and gpaths
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

#define my_layer_init(Layer)			\
  Layer=layer_create(bounds); \
  layer_set_update_proc(Layer, Layer ## _update); \
  layer_add_child(window_layer, Layer);

  minute_hand_path=gpath_create(&minute_hand_points);
  GPoint center = grect_center_point(&bounds);
  gpath_move_to(minute_hand_path, center);
  my_layer_init(minute_dial);
  my_layer_init(minute_hand);

  my_layer_init(hour_dial);

  hour_hand_path=gpath_create(&hour_hand_points);
  bounds = layer_get_bounds(hour_dial);
  center = grect_center_point(&bounds);
  gpath_move_to(hour_hand_path, center);

  GRect frame=layer_get_frame(hour_dial);
  hour_hand=layer_create(frame);
  layer_set_update_proc(hour_hand, hour_hand_update);
  layer_add_child(hour_dial, hour_hand);
}

static void window_unload(Window *window) {
  // destroy all layers and gpaths
  layer_destroy(minute_dial);
  layer_destroy(minute_hand);
  layer_destroy(hour_dial);
  layer_destroy(hour_hand);
  layer_destroy(second_dial);
  layer_destroy(second_hand);
  gpath_destroy(hour_hand_path);
  gpath_destroy(minute_hand_path);

}

static void init(void) {
  // create window
  window = window_create();
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  //Layer *root_layer = window_get_root_layer(window);
  const bool animated = true;
  window_stack_push(window, animated);
  tick_timer_service_subscribe(MINUTE_UNIT, handle_tick);
}

static void deinit(void) {
  // destroy window
  window_destroy(window);
}

int main(void) {
  init();
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", window);
  app_event_loop();
  deinit();
  return(0);
}
