#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"

#include "config.h" // from build-all

#define APP_NAME "Floating Dials"
#define MY_UUID { 0xA7, 0x13, 0xC7, 0x01, 0x7B, 0xB1, 0x4E, 0x19, 0x9B, 0xEF, 0x5A, 0xF2, 0x15, 0xD6, 0xFC, 0x51 }
PBL_APP_INFO(MY_UUID, APP_NAME " " VERSION, "Koishi", MAJOR, MINOR, 
             RESOURCE_ID_IMAGE_MENU_ICON, 
             APP_INFO_WATCH_FACE);

#ifndef INVERTED
#define BACKGROUND GColorBlack
#define FOREGROUND GColorWhite
#else
#define BACKGROUND GColorWhite
#define FOREGROUND GColorBlack
#endif

#define ROCKSHOT false
//#define ROCKSHOT true
#if ROCKSHOT
#include "rockshot.h"
#endif

Window window;

Layer minute_dial, minute_hand, hour_dial, hour_hand, second_dial, second_hand;

GPoint rotate_point(int angle, int radius, GPoint center) {
  GPoint point;
  int32_t second_angle = TRIG_MAX_ANGLE * angle / 60;
  point.y = (-cos_lookup(second_angle) * radius / TRIG_MAX_RATIO) + center.y;
  point.x = (sin_lookup(second_angle) * radius / TRIG_MAX_RATIO) + center.x;
  return point;
}

#define MINUTE_RADIUS 71
#define FIFTEEN_MARK_LENGTH 5

void minute_dial_update(Layer *layer, GContext* gctx) {
  graphics_context_set_stroke_color(gctx, FOREGROUND);
  GPoint center = grect_center_point(&layer->frame);
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
    GPoint outer=rotate_point(angle, radius, center);
    GPoint inner=rotate_point(angle, radius - length, center);
    graphics_draw_line (gctx, outer, inner);
  }
  for (int hour=0; hour<12; hour++){
    
  }
}

GPath minute_hand_path;

void minute_hand_update(Layer *layer, GContext* gctx) {
  //graphics_context_set_fill_color(gctx, FOREGROUND);
  //graphics_context_set_stroke_color(gctx, BACKGROUND);
  graphics_context_set_fill_color(gctx, BACKGROUND);
  graphics_context_set_stroke_color(gctx, FOREGROUND);
  PblTm time;
  get_time(&time);
  gpath_rotate_to(&minute_hand_path, TRIG_MAX_ANGLE * time.tm_min / 60);
  gpath_draw_filled(gctx, &minute_hand_path);
  gpath_draw_outline(gctx, &minute_hand_path);
}

static const GPathInfo minute_hand_points = {
  3,
  (GPoint []) {
    { -15, 0 },
    { 15, 0 },
    { 0, (FIFTEEN_MARK_LENGTH - MINUTE_RADIUS) }
  }
};

void handle_init(AppContextRef ctx) {
  window_init(&window, "Floating Dials");
  window_stack_push(&window, true /* Animated */);

  window_set_background_color(&window, BACKGROUND);
  resource_init_current_app(&APP_RESOURCES);

  gpath_init(&minute_hand_path, &minute_hand_points);
  const GPoint center = grect_center_point(&window.layer.bounds);
  gpath_move_to(&minute_hand_path, center);

#define my_layer_init(Layer) \
  layer_init(&Layer, window.layer.frame); \
  Layer.update_proc = &Layer ## _update; \
  layer_add_child(&window.layer, &Layer);

  my_layer_init(minute_dial);
  my_layer_init(minute_hand);

#if ROCKSHOT
  rockshot_init(ctx);
#endif
}

static void handle_tick(AppContextRef ctx, PebbleTickEvent* t) {
  layer_mark_dirty(&minute_hand);
}

void pbl_main(void *params) {
  PebbleAppHandlers handlers = {
#if ROCKSHOT
    .messaging_info = {
      .buffer_sizes = {
        .inbound = 64, // inbound buffer size in bytes
        .outbound = 256, // outbound buffer size in bytes
      },
    },
#endif
    .init_handler = &handle_init,
    .tick_info = {
      .tick_handler = &handle_tick,
      .tick_units = MINUTE_UNIT
    }
  };
#if ROCKSHOT
  rockshot_main(&handlers);
#endif
  app_event_loop(params, &handlers);
}
