#include <pebble.h>
#include "hrv_math.h"
#include "storage.h"
#include "sleep_stage.h"

static Window *s_window;
static Layer *s_canvas;
static bool s_recording = false;
static uint32_t s_hr_events = 0;
static uint32_t s_hrv_events = 0;
static uint16_t s_last_ppi = 0;
static uint16_t s_last_hr = 0;
static HrvBuffer s_live_buf;
static HrvBuffer s_minute_buf;
static HrvBuffer s_night_buf;
static time_t s_session_start = 0;
static uint32_t s_night_baseline_var = 0;
static uint16_t s_mins[4] = {0, 0, 0, 0};
static uint8_t s_awake_streak = 0;
static SleepStage s_last_stage = StageLight;
static AppTimer *s_ui_timer = NULL;
typedef enum { MODE_IDLE, MODE_RECORDING, MODE_RESULTS, MODE_HYPNO } ScreenMode;
static ScreenMode s_mode = MODE_IDLE;
static time_t s_session_end = 0;
static void prv_click_config(void *ctx);
#define AWAKE_DEBOUNCE 3

static bool s_hrv_on = true;
static void prv_set_hrv(bool on) {
  if (on == s_hrv_on) return;
  s_hrv_on = on;
  health_service_set_hrv_sample_period(on ? 1 : 0);
}

static void prv_close_minute(void) {
  if (!s_recording) return;
  EpochRecord rec;
  uint16_t total = s_minute_buf.count + (uint16_t)s_minute_buf.rejected;
  rec.mean_ppi = hrv_mean_ppi(&s_minute_buf);
  rec.rmssd = hrv_rmssd(&s_minute_buf);
  rec.beat_count = (s_minute_buf.count > 255) ? 255 : (uint8_t)s_minute_buf.count;
  rec.quality_pct = (total > 0) ? (uint8_t)((s_minute_buf.count * 100) / total) : 0;
  rec.reserved = 0;
  SleepStage st = sleep_stage_classify(&s_minute_buf, s_night_baseline_var);
  if (st == StageAwake) {
    s_awake_streak++;
    if (s_awake_streak < AWAKE_DEBOUNCE) st = s_last_stage;
  } else {
    s_awake_streak = 0;
  }
  s_last_stage = st;
  rec.stage = (uint8_t)st;
  s_mins[st]++;
  storage_epoch_write(&rec);
  if (s_night_buf.count >= 60 && s_night_baseline_var == 0) {
    s_night_baseline_var = hrv_ppi_variance(&s_night_buf);
  }
  hrv_buf_reset(&s_minute_buf);
}

static void prv_tick_handler(struct tm *tick_time, TimeUnits changed) {
  prv_close_minute();
  if (s_recording) prv_set_hrv(true);
  layer_mark_dirty(s_canvas);
}

static void prv_health_handler(HealthEventType event, void *context) {
  if (event == HealthEventHeartRateUpdate) {
    s_hr_events++;
    HealthValue hr = health_service_peek_current_value(HealthMetricHeartRateRawBPM);
    if (hr > 0) s_last_hr = (uint16_t)hr;
  } else if ((int)event == 5) {
    s_hrv_events++;
    uint16_t ppi = (uint16_t)health_service_peek_hrv_ppi_ms();
    if (ppi > 0) {
      s_last_ppi = ppi;
      uint32_t now = (uint32_t)time(NULL);
      hrv_buf_add(&s_live_buf, ppi, 1, now);
      if (s_recording) {
        hrv_buf_add(&s_minute_buf, ppi, 1, now);
        hrv_buf_add(&s_night_buf, ppi, 1, now);
      }
    }
  }
  if (s_mode == MODE_IDLE) layer_mark_dirty(s_canvas);
}

static void prv_ui_tick(void *data) {
  s_ui_timer = NULL;
  if (!s_recording) return;
  layer_mark_dirty(s_canvas);
  s_ui_timer = app_timer_register(1000, prv_ui_tick, NULL);
}

static void prv_start_recording(void) {
  s_recording = true;
  prv_set_hrv(true);
  s_session_start = time(NULL);
  s_night_baseline_var = 0;
  s_awake_streak = 0;
  s_last_stage = StageLight;
  for (int i = 0; i < 4; i++) s_mins[i] = 0;
  hrv_buf_reset(&s_minute_buf);
  hrv_buf_reset(&s_night_buf);
  storage_session_start();
  s_mode = MODE_RECORDING;
  window_set_click_config_provider(s_window, prv_click_config);
  layer_mark_dirty(s_canvas);
}

static void prv_stop_recording(void) {
  prv_close_minute();
  s_recording = false;
  s_session_end = time(NULL);
  NightSummary ns;
  ns.rmssd = hrv_rmssd(&s_night_buf);
  ns.sdnn = hrv_sdnn(&s_night_buf);
  ns.mean_ppi = hrv_mean_ppi(&s_night_buf);
  ns.epoch_count = storage_epoch_count();
  int deep_sec = (int)health_service_sum(HealthMetricSleepRestfulSeconds,
                                         s_session_start, s_session_end);
  uint16_t deep_min = (deep_sec > 0) ? (uint16_t)(deep_sec / 60) : 0;
  s_mins[StageDeep] = deep_min;
  if (deep_min <= s_mins[StageLight]) s_mins[StageLight] -= deep_min;
  else s_mins[StageLight] = 0;
  ns.mins_awake = s_mins[StageAwake];
  ns.mins_light = s_mins[StageLight];
  ns.mins_deep = s_mins[StageDeep];
  ns.mins_rem = s_mins[StageREM];
  ns.beats = s_night_buf.total_accepted;
  ns.rejected = s_night_buf.rejected;
  ns.rej_range = s_night_buf.rej_range;
  ns.rej_jump = s_night_buf.rej_jump;
  ns.start_time = s_session_start;
  ns.end_time = s_session_end;
  if (ns.epoch_count >= 30) storage_night_save(&ns);
  s_mode = MODE_RESULTS;
  window_set_click_config_provider(s_window, prv_click_config);
  layer_mark_dirty(s_canvas);
}

static void prv_draw_idle(Layer *layer, GContext *ctx) {
  GRect b = layer_get_bounds(layer);
  graphics_context_set_text_color(ctx, GColorBlack);
  char line[64];
  int y = 2;
  snprintf(line, sizeof(line), "Recovery idle");
  graphics_draw_text(ctx, line, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD),
    GRect(4, y, b.size.w - 8, 30), GTextOverflowModeTrailingEllipsis,
    GTextAlignmentLeft, NULL); y += 30;
  uint32_t dur = s_recording ? (uint32_t)(time(NULL) - s_session_start) : 0;
  snprintf(line, sizeof(line), "Dur %lu:%02lu", (unsigned long)(dur/60), (unsigned long)(dur%60));
  graphics_draw_text(ctx, line, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD),
    GRect(4, y, b.size.w - 8, 26), GTextOverflowModeTrailingEllipsis,
    GTextAlignmentLeft, NULL); y += 26;
  snprintf(line, sizeof(line), "HR %u  PPI %u", s_last_hr, s_last_ppi);
  graphics_draw_text(ctx, line, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD),
    GRect(4, y, b.size.w - 8, 26), GTextOverflowModeTrailingEllipsis,
    GTextAlignmentLeft, NULL); y += 26;
  snprintf(line, sizeof(line), "RMSSD %u", hrv_rmssd(&s_live_buf));
  graphics_draw_text(ctx, line, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD),
    GRect(4, y, b.size.w - 8, 26), GTextOverflowModeTrailingEllipsis,
    GTextAlignmentLeft, NULL); y += 26;
  snprintf(line, sizeof(line), "ev H%lu V%lu", (unsigned long)s_hr_events,
    (unsigned long)s_hrv_events);
  graphics_draw_text(ctx, line, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD),
    GRect(4, y, b.size.w - 8, 26), GTextOverflowModeTrailingEllipsis,
    GTextAlignmentLeft, NULL); y += 26;
  snprintf(line, sizeof(line), "A%u L%u D%u R%u", s_mins[0], s_mins[1],
    s_mins[2], s_mins[3]);
  graphics_draw_text(ctx, line, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD),
    GRect(4, y, b.size.w - 8, 26), GTextOverflowModeTrailingEllipsis,
    GTextAlignmentLeft, NULL);
}

static void prv_draw_recording(Layer *layer, GContext *ctx) {
  GRect b = layer_get_bounds(layer);
  graphics_context_set_text_color(ctx, GColorBlack);
  char t[8];
  clock_copy_time_string(t, sizeof(t));
  graphics_draw_text(ctx, t, fonts_get_system_font(FONT_KEY_LECO_42_NUMBERS),
    GRect(0, 40, b.size.w, 48), GTextOverflowModeTrailingEllipsis,
    GTextAlignmentCenter, NULL);
  graphics_draw_text(ctx, "Recording", fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD),
    GRect(4, 100, b.size.w - 8, 28), GTextOverflowModeTrailingEllipsis,
    GTextAlignmentCenter, NULL);
  graphics_draw_text(ctx, "Hold Select 1.5s to stop",
    fonts_get_system_font(FONT_KEY_GOTHIC_18),
    GRect(4, 132, b.size.w - 8, 40), GTextOverflowModeWordWrap,
    GTextAlignmentCenter, NULL);
}

static void prv_draw_results(Layer *layer, GContext *ctx) {
  GRect b = layer_get_bounds(layer);
  graphics_context_set_text_color(ctx, GColorBlack);
  char line[64];
  int y = 2;
  graphics_draw_text(ctx, "Sleep Recorded",
    fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD),
    GRect(4, y, b.size.w - 8, 30), GTextOverflowModeTrailingEllipsis,
    GTextAlignmentLeft, NULL); y += 32;
  uint32_t dur = (s_session_end > s_session_start)
    ? (uint32_t)(s_session_end - s_session_start) : 0;
  snprintf(line, sizeof(line), "Dur %luh %02lum",
    (unsigned long)(dur/3600), (unsigned long)((dur%3600)/60));
  graphics_draw_text(ctx, line, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD),
    GRect(4, y, b.size.w - 8, 26), GTextOverflowModeTrailingEllipsis,
    GTextAlignmentLeft, NULL); y += 26;
  snprintf(line, sizeof(line), "Awake %u  REM %u", s_mins[StageAwake], s_mins[StageREM]);
  graphics_draw_text(ctx, line, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD),
    GRect(4, y, b.size.w - 8, 26), GTextOverflowModeTrailingEllipsis,
    GTextAlignmentLeft, NULL); y += 26;
  snprintf(line, sizeof(line), "Light %u  Deep %u", s_mins[StageLight], s_mins[StageDeep]);
  graphics_draw_text(ctx, line, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD),
    GRect(4, y, b.size.w - 8, 26), GTextOverflowModeTrailingEllipsis,
    GTextAlignmentLeft, NULL); y += 26;
  snprintf(line, sizeof(line), "RMSSD %u", hrv_rmssd(&s_night_buf));
  graphics_draw_text(ctx, line, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD),
    GRect(4, y, b.size.w - 8, 26), GTextOverflowModeTrailingEllipsis,
    GTextAlignmentLeft, NULL); y += 26;
  graphics_draw_text(ctx, "Down: graph",
    fonts_get_system_font(FONT_KEY_GOTHIC_18),
    GRect(4, y, b.size.w - 8, 22), GTextOverflowModeTrailingEllipsis,
    GTextAlignmentLeft, NULL);
}

static GColor prv_stage_color(uint8_t stage) {
  switch (stage) {
    case StageAwake: return GColorRed;
    case StageREM:   return GColorPurple;
    case StageLight: return GColorCyan;
    case StageDeep:  return GColorDukeBlue;
    default:         return GColorLightGray;
  }
}

static void prv_draw_hypno(Layer *layer, GContext *ctx) {
  GRect b = layer_get_bounds(layer);
  graphics_context_set_text_color(ctx, GColorBlack);
  graphics_draw_text(ctx, "Sleep Stages",
    fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD),
    GRect(4, 0, b.size.w - 8, 22), GTextOverflowModeTrailingEllipsis,
    GTextAlignmentLeft, NULL);
  int top = 26;
  int bottom = b.size.h - 8;
  int gh = bottom - top;
  uint16_t n = storage_epoch_count();
  if (n == 0) return;
  int plot_w = b.size.w - 8;
  // depth fraction: Awake tallest -> Deep shortest
  for (uint16_t i = 0; i < n; i++) {
    EpochRecord rec;
    if (!storage_epoch_read(i, &rec)) continue;
    int h;
    switch (rec.stage) {
      case StageAwake: h = gh;           break;
      case StageREM:   h = (gh * 3) / 4; break;
      case StageLight: h = gh / 2;       break;
      case StageDeep:  h = gh / 4;       break;
      default:         h = gh / 8;       break;
    }
    int x0 = 4 + (int)((uint32_t)i * plot_w / n);
    int x1 = 4 + (int)((uint32_t)(i + 1) * plot_w / n);
    int w = (x1 > x0) ? (x1 - x0) : 1;
    graphics_context_set_fill_color(ctx, prv_stage_color(rec.stage));
    graphics_fill_rect(ctx, GRect(x0, bottom - h, w, h), 0, GCornerNone);
  }
}

static void prv_canvas_update(Layer *layer, GContext *ctx) {
  switch (s_mode) {
    case MODE_RECORDING: prv_draw_recording(layer, ctx); break;
    case MODE_RESULTS:   prv_draw_results(layer, ctx);   break;
    case MODE_HYPNO:     prv_draw_hypno(layer, ctx);     break;
    case MODE_IDLE:
    default:             prv_draw_idle(layer, ctx);      break;
  }
}


static void prv_noop(ClickRecognizerRef r, void *ctx) { }

static void prv_idle_exit(ClickRecognizerRef r, void *ctx) {
  window_stack_pop(true);
}
static void prv_start_long(ClickRecognizerRef r, void *ctx) {
  if (!s_recording) { prv_start_recording(); }
}
static void prv_stop_long(ClickRecognizerRef r, void *ctx) {
  if (s_recording) { prv_stop_recording(); }
}
static void prv_results_down(ClickRecognizerRef r, void *ctx) {
  s_mode = MODE_HYPNO;
  window_set_click_config_provider(s_window, prv_click_config);
  layer_mark_dirty(s_canvas);
}
static void prv_hypno_back_to_results(ClickRecognizerRef r, void *ctx) {
  s_mode = MODE_RESULTS;
  window_set_click_config_provider(s_window, prv_click_config);
  layer_mark_dirty(s_canvas);
}
static void prv_results_to_idle(ClickRecognizerRef r, void *ctx) {
  s_mode = MODE_IDLE;
  window_set_click_config_provider(s_window, prv_click_config);
  layer_mark_dirty(s_canvas);
}

static void prv_click_config(void *ctx) {
  switch (s_mode) {
    case MODE_IDLE:
      window_long_click_subscribe(BUTTON_ID_SELECT, 1500, prv_start_long, NULL);
      window_single_click_subscribe(BUTTON_ID_BACK, prv_idle_exit);
      break;
    case MODE_RECORDING:
      window_long_click_subscribe(BUTTON_ID_SELECT, 1500, prv_stop_long, NULL);
      window_single_click_subscribe(BUTTON_ID_BACK, prv_noop);
      window_long_click_subscribe(BUTTON_ID_BACK, 700, prv_noop, NULL);
      break;
    case MODE_RESULTS:
      window_single_click_subscribe(BUTTON_ID_DOWN, prv_results_down);
      window_single_click_subscribe(BUTTON_ID_BACK, prv_results_to_idle);
      break;
    case MODE_HYPNO:
      window_single_click_subscribe(BUTTON_ID_UP, prv_hypno_back_to_results);
      window_single_click_subscribe(BUTTON_ID_BACK, prv_hypno_back_to_results);
      break;
  }
}

static void prv_window_load(Window *w) {
  Layer *root = window_get_root_layer(w);
  GRect b = layer_get_bounds(root);
  s_canvas = layer_create(b);
  layer_set_update_proc(s_canvas, prv_canvas_update);
  layer_add_child(root, s_canvas);
}

static void prv_window_unload(Window *w) {
  layer_destroy(s_canvas);
}

static void prv_init(void) {
  s_window = window_create();
  window_set_click_config_provider(s_window, prv_click_config);
  window_set_window_handlers(s_window, (WindowHandlers){
    .load = prv_window_load, .unload = prv_window_unload });
  window_stack_push(s_window, true);
  hrv_buf_reset(&s_live_buf);
  hrv_buf_reset(&s_minute_buf);
  hrv_buf_reset(&s_night_buf);
  health_service_events_subscribe(prv_health_handler, NULL);
  tick_timer_service_subscribe(MINUTE_UNIT, prv_tick_handler);
}

static void prv_deinit(void) {
  if (s_recording) prv_stop_recording();
  prv_set_hrv(false);
  tick_timer_service_unsubscribe();
  health_service_events_unsubscribe();
  window_destroy(s_window);
}

int main(void) {
  prv_init();
  app_event_loop();
  prv_deinit();
}
