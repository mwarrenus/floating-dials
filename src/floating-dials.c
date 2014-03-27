#include <pebble.h>
#include "autoconfig.h"

int HOURS;

GColor BACKGROUND;
GColor FOREGROUND;

static Window *window;
static Layer *minute_dial, *minute_hand, *hour_dial, *hour_hand, *second_dial, *second_hand, *date_dial, *date_hand;

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

#define SECOND_RADIUS 22
#define SECOND_OFFSET 24

bool wasseconds = false;
bool wasdate = false;

void hour_dial_update(Layer *layer, GContext* gctx) {
  int MAX_HOURS_LABEL;
  int HOUR_DIVISOR;
  int HOUR_COUNTER;
  if(HOURS == 12){
    MAX_HOURS_LABEL=12;
    HOUR_DIVISOR=2;
  } else {
    MAX_HOURS_LABEL=0;
    HOUR_DIVISOR=1;
    HOUR_COUNTER=3;
  }
  static char hour_text[] = "12";
  GRect frame=layer_get_frame(layer);
  GPoint center = grect_center_point(&frame);
  graphics_context_set_fill_color(gctx, BACKGROUND);
  int32_t radius = HOUR_RADIUS ;
  graphics_fill_circle(gctx, center, radius);
  radius -= HOUR_BORDER;
  graphics_context_set_stroke_color(gctx, FOREGROUND);
  graphics_context_set_text_color(gctx, FOREGROUND);
  for (int angle=0; angle < HOURS * HOUR_DIVISOR; angle++){
    int32_t length;
    if(angle % ((HOURS / HOUR_MARKS) * HOUR_DIVISOR) == 0) {
      length = THREE_MARK_LENGTH;
    } else
      if( 
	 ( (HOURS == 12) && (angle % HOUR_DIVISOR == 0) )	\
	 || \
	 ( (HOURS == 24) && (angle % HOUR_COUNTER == 0) ) \
        ) {
      length = ONE_MARK_LENGTH;
    } else {
      length = DIVISOR_MARK_LENGTH;
    }
    GPoint outer=rotate_point(angle, HOURS * HOUR_DIVISOR, radius, center);
    GPoint inner=rotate_point(angle, HOURS * HOUR_DIVISOR, radius - length, center);
    graphics_draw_line(gctx, outer, inner);

    if (angle % (((HOURS / HOUR_MARKS)) * HOUR_DIVISOR) == 0) {
      GPoint text_center=rotate_point(angle, HOURS * HOUR_DIVISOR, radius - THREE_MARK_LENGTH - 10 , center);
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


void second_dial_update(Layer *layer, GContext* gctx) {
  //APP_LOG(APP_LOG_LEVEL_DEBUG, "second_dial_update");

  GRect frame=layer_get_frame(layer);
  GPoint center = grect_center_point(&frame);
  graphics_context_set_fill_color(gctx, BACKGROUND);
  int32_t radius = SECOND_RADIUS ;
  graphics_fill_circle(gctx, center, radius);
  radius -= HOUR_BORDER;
  graphics_context_set_stroke_color(gctx, FOREGROUND);
  graphics_context_set_text_color(gctx, FOREGROUND);
  for (int angle=0; angle < 60; angle += 5){
    int32_t length = DIVISOR_MARK_LENGTH;
    if(angle % 15 == 0) {
      length = ONE_MARK_LENGTH;
    } 
    GPoint outer=rotate_point(angle, 60, radius, center);
    GPoint inner=rotate_point(angle, 60, radius - length, center);
    graphics_draw_line(gctx, outer, inner);
  }
}


void date_dial_update(Layer *layer, GContext* gctx) {
  //APP_LOG(APP_LOG_LEVEL_DEBUG, "date_dial_update");

  GRect frame=layer_get_frame(layer);
  GPoint center = grect_center_point(&frame);
  graphics_context_set_fill_color(gctx, BACKGROUND);
  int32_t radius = SECOND_RADIUS ;
  graphics_fill_circle(gctx, center, radius);
  radius -= HOUR_BORDER;
  graphics_context_set_stroke_color(gctx, FOREGROUND);
  graphics_context_set_text_color(gctx, FOREGROUND);
  for (int angle=0; angle < 31; angle += 2){
    int32_t length = DIVISOR_MARK_LENGTH;
    if(angle % 8 == 0) {
      length = ONE_MARK_LENGTH;
    } 
    GPoint outer=rotate_point(angle, 32, radius, center);
    GPoint inner=rotate_point(angle, 32, radius - length, center);
    graphics_draw_line(gctx, outer, inner);
  }
  radius = SECOND_RADIUS / 2;
  for (int angle=0; angle < 7; angle += 1){
    int32_t length = DIVISOR_MARK_LENGTH;
    if((angle == 0) || (angle == 6) ) {
      length = ONE_MARK_LENGTH;
    } 
    GPoint outer=rotate_point(angle, 7, radius, center);
    GPoint inner=rotate_point(angle, 7, radius - length, center);
    graphics_draw_line(gctx, outer, inner);
  }
}


static GPath *minute_hand_path, *hour_hand_path;

void minute_hand_update(Layer *layer, GContext* gctx) {
  graphics_context_set_fill_color(gctx, FOREGROUND);
  graphics_context_set_stroke_color(gctx, BACKGROUND);
  time_t now = time(NULL);
  struct tm *time = localtime(&now);
  gpath_rotate_to(minute_hand_path, TRIG_MAX_ANGLE * time->tm_min / 60);
  gpath_draw_filled(gctx, minute_hand_path);
  gpath_draw_outline(gctx, minute_hand_path);
  GPoint hour=rotate_point(time->tm_min, 60, HOUR_RADIUS, GPoint (0, 0));
  layer_set_bounds(hour_dial,GRect (-hour.x, -hour.y, 144, 168));	
  GPoint second=rotate_point((time->tm_min + 15) % 60, 60, SECOND_RADIUS + SECOND_OFFSET, GPoint (0, 0));
  layer_set_bounds(second_dial,GRect (second.x, second.y, 144, 168));	
  GPoint date=rotate_point((time->tm_min - 15) % 60, 60, SECOND_RADIUS + SECOND_OFFSET, GPoint (0, 0));
  layer_set_bounds(date_dial,GRect (date.x, date.y, 144, 168));	
}

void hour_hand_update(Layer *layer, GContext* gctx) {
  graphics_context_set_fill_color(gctx, FOREGROUND);
  graphics_context_set_stroke_color(gctx, BACKGROUND);
  time_t now = time(NULL);
  struct tm *time = localtime(&now);
#ifdef TEST
  gpath_rotate_to(hour_hand_path, TRIG_MAX_ANGLE * time->tm_sec / 60);
#else
  //  gpath_rotate_to(hour_hand_path, TRIG_MAX_ANGLE * ( (time->tm_hour + (time->tm_min / 60)) / 12 ) );
  gpath_rotate_to(hour_hand_path, (TRIG_MAX_ANGLE * (((time->tm_hour % HOURS) * 6) + (time->tm_min / 10))) / (HOURS * 6));
#endif
  gpath_draw_filled(gctx, hour_hand_path);
  gpath_draw_outline(gctx, hour_hand_path);
}

void second_hand_update(Layer *layer, GContext* gctx) {
  //APP_LOG(APP_LOG_LEVEL_DEBUG, "second_hand_update");

  graphics_context_set_fill_color(gctx, FOREGROUND);
  time_t now = time(NULL);
  struct tm *time = localtime(&now);
  //gpath_rotate_to(second_hand_path, TRIG_MAX_ANGLE * time->tm_sec / 60);
  //gpath_draw_filled(gctx, second_hand_path);
  //graphics_context_set_stroke_color(gctx, BACKGROUND);
  //gpath_draw_outline(gctx, second_hand_path);

  GRect frame=layer_get_frame(layer);
  GPoint center = grect_center_point(&frame);
  GPoint outer=rotate_point(time->tm_sec, 60, (SECOND_RADIUS - THREE_MARK_LENGTH), center);
  graphics_context_set_stroke_color(gctx, FOREGROUND);
  graphics_draw_line(gctx, outer, center);
}


void date_hand_update(Layer *layer, GContext* gctx) {
  //APP_LOG(APP_LOG_LEVEL_DEBUG, "date_hand_update");
  graphics_context_set_fill_color(gctx, FOREGROUND);
  time_t now = time(NULL);
  struct tm *time = localtime(&now);
  GRect frame=layer_get_frame(layer);
  GPoint center = grect_center_point(&frame);
  GPoint outer=rotate_point(time->tm_mday, 32, (SECOND_RADIUS - THREE_MARK_LENGTH), center);
  graphics_context_set_stroke_color(gctx, FOREGROUND);
  graphics_draw_line(gctx, outer, center);
  outer=rotate_point(time->tm_wday, 7, ((SECOND_RADIUS / 2) - THREE_MARK_LENGTH), center);
  graphics_context_set_stroke_color(gctx, FOREGROUND);
  graphics_draw_line(gctx, outer, center);
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
  //APP_LOG(APP_LOG_LEVEL_DEBUG, "handle_tick");
  //APP_LOG(APP_LOG_LEVEL_DEBUG, "handle_tick getSeconds:%d wasseconds:%d", getSeconds(), wasseconds);
  if (wasseconds && (units_changed & SECOND_UNIT)) {
    //APP_LOG(APP_LOG_LEVEL_DEBUG, "handle_tick layer_mark_dirty(second_hand)");
    layer_mark_dirty(second_hand);
  }
  if (units_changed & MINUTE_UNIT) {
    layer_mark_dirty(minute_hand);
    if (units_changed & HOUR_UNIT) {
      layer_mark_dirty(hour_hand);
      if (wasdate && (units_changed & DAY_UNIT)) {
	layer_mark_dirty(date_hand);
      }
    }
  }
}


static void window_load(Window *window) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "window_load");
  APP_LOG(APP_LOG_LEVEL_DEBUG, "window_load start getSeconds:%d wasseconds:%d", getSeconds(), wasseconds);

  if(getBackground()){
    BACKGROUND=GColorWhite;
    FOREGROUND=GColorBlack;
  } else {
    BACKGROUND=GColorBlack;
    FOREGROUND=GColorWhite;
  }
  if(getHourdialtype()){
    HOURS=24;
  } else {
    HOURS=12;
  }
  window_set_background_color(window, BACKGROUND);

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

  my_layer_init(second_dial);
  bounds = layer_get_bounds(second_dial);
  frame=layer_get_frame(second_dial);
  second_hand=layer_create(frame);
  layer_set_update_proc(second_hand, second_hand_update);
  layer_add_child(second_dial, second_hand);

  my_layer_init(date_dial);
  bounds = layer_get_bounds(date_dial);
  frame=layer_get_frame(date_dial);
  date_hand=layer_create(frame);
  layer_set_update_proc(date_hand, date_hand_update);
  layer_add_child(date_dial, date_hand);

  if(getSeconds()){
    APP_LOG(APP_LOG_LEVEL_DEBUG, "window_load seconds true");
    wasseconds = true;
    tick_timer_service_subscribe(SECOND_UNIT, handle_tick);
  } else {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "window_load seconds false");
    wasseconds = false;
    layer_set_hidden((Layer *)second_dial, true);
    tick_timer_service_subscribe(MINUTE_UNIT, handle_tick);
  } 
  if(getDate()){
    wasdate = true;
  } else {
    wasdate = false;
    layer_set_hidden((Layer *)date_dial, true);
  } 
  APP_LOG(APP_LOG_LEVEL_DEBUG, "window_load end getSeconds:%d wasseconds:%d", getSeconds(), wasseconds);

}

static void window_unload(Window *window) {
  //APP_LOG(APP_LOG_LEVEL_DEBUG, "window_unload getSeconds:%d wasseconds:%d", getSeconds(), wasseconds);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "window_unload");
  // destroy all layers and gpaths
  tick_timer_service_unsubscribe();
  layer_destroy(minute_dial);
  layer_destroy(minute_hand);
  gpath_destroy(minute_hand_path);
  layer_destroy(hour_dial);
  layer_destroy(hour_hand);
  gpath_destroy(hour_hand_path);
  layer_destroy(second_dial);
  layer_destroy(second_hand);
  layer_destroy(date_dial);
  layer_destroy(date_hand);
}

static void in_received_handler(DictionaryIterator *iter, void *context) {
  autoconfig_in_received_handler(iter, context);
  //APP_LOG(APP_LOG_LEVEL_DEBUG, "in_received_handler background:%d seconds:%d hourdialtype:%d handwidth:%d", getBackground(), getSeconds(), getHourdialtype(), (int)getHandwidth());
  APP_LOG(APP_LOG_LEVEL_DEBUG, "in_received_handler background:%d hourdialtype:%d seconds:%d", getBackground(), getHourdialtype(), getSeconds());

  //update display
  // crashfest:
  //window_unload(window);
  //window_load(window);
}

static void init(void) {
  autoconfig_init();
  //APP_LOG(APP_LOG_LEVEL_DEBUG, "init background:%d seconds:%d hourdialtype:%d handwidth:%d", getBackground(), getSeconds(), getHourdialtype(), (int)getHandwidth());
  APP_LOG(APP_LOG_LEVEL_DEBUG, "init background:%d hourdialtype:%d seconds:%d", getBackground(), getHourdialtype(), getSeconds());

  // create window
  window = window_create();
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  const bool animated = true;
  window_stack_push(window, animated);
  app_message_register_inbox_received(in_received_handler);
}

static void deinit(void) {
  //APP_LOG(APP_LOG_LEVEL_DEBUG, "deinit getSeconds:%d wasseconds:%d", getSeconds(), wasseconds);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "deinit background:%d hourdialtype:%d seconds:%d", getBackground(), getHourdialtype(), getSeconds());
  autoconfig_deinit();
  window_destroy(window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
  return(0);
}
