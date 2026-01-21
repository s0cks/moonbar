#include "hypr/hyprctl.h"
#include "hypr_client.h"
#include "log.h"
#include "moonbar.h"
#include "state_gtk.h"
#include "state_lua.h"
#include "util.h"
#include "uv_gsource.h"

void mbar_error(BarApp* app, const char* err) {
  ASSERT(app);
  ASSERT(err);
  fprintf(stderr, "bar state error: %s\n", err);
  exit(1);
}

void mbar_publish(BarApp* app, const char* event) {
  EventRoute* root = event_route_search(app->events, event);
  if (!root)
    return;
#define L app->L
  event_route_call(L, root);
#undef L
}

static inline void on_tick(uv_timer_t* handle) {
  BarApp* app = (BarApp*)handle->data;
  mbar_on_tick(app);
}

static inline bool is_changed(const int events) {
  return (events & UV_CHANGE);
}

static inline void on_style_changed(uv_fs_event_t* handle, const char* filename, int events, int status) {
  BarApp* app = (BarApp*)handle->data;
  if (status < 0) {
    mbar_error(app, "failed to watch style changes");
    return;
  }

  mbar_load_style(app);
  uv_fs_event_stop(handle);
  uv_fs_event_init(app->loop, &app->style_watcher);
  mbar_start_style_watcher(app);
}

bool mbar_is_style_watcher_running(BarApp* app) {
  ASSERT(app);
  return uv_is_active((uv_handle_t*)&app->style_watcher);
}

void mbar_start_style_watcher(BarApp* app) {
  if (mbar_is_style_watcher_running(app))
    return;
  const char* filename = mbar_config_get_style(app);
  DLOG_F("starting style filewatcher for %s\n", filename);
  const int status = uv_fs_event_start(&app->style_watcher, &on_style_changed, filename, 0);
  if (status < 0)
    mbar_error(app, "failed to create style watcher");
}

static inline bool get_hypr_instance_path(char* result, const size_t result_len) {
  const char* his = getenv("HYPRLAND_INSTANCE_SIGNATURE");
  const char* xdg_runtime_dir = getenv("XDG_RUNTIME_DIR");
  if (!his || !xdg_runtime_dir)
    return false;
  memset(result, 0, result_len);
  snprintf(result, result_len, "%s/hypr/%s", xdg_runtime_dir, his);
  return true;
}

bool mbar_app_init(BarApp* app, int argc, char** argv) {
  ASSERT(app);
  app->argc = argc;
  app->argv = argv;
  app->loop = uv_loop_new();
  app->home = mbar_get_config_dir();
  app->events = event_route_new();
  app->next_tick_listeners = nullptr;
  app->box = nullptr;

  uv_timer_init(app->loop, &app->timer);
  uv_timer_start(&app->timer, on_tick, 0, 1000);
  app->timer.data = app;

  uv_fs_event_init(app->loop, &app->style_watcher);
  app->style_watcher.data = app;

  // Hyprctl hyprctl;
  // hyprctl_init(app->loop, &hyprctl);
  // hyprctl_sends(&hyprctl, "dispatch workspace 1");

  app->source = uv_gsource_init(app->loop);
  mbarL_init(app);
  mbar_init_gtk_app(app);
  return true;
}

bool mbar_app_run(BarApp* app) {
  int status = g_application_run(G_APPLICATION(app->app), app->argc, app->argv);
  // TODO(@s0cks): check status
  return true;
}

void mbar_app_free(BarApp* app) {
  ASSERT(app);
  if (app->app)
    g_object_unref(app->app);
  mbarL_close(app);
  if (app->home)
    free(app->home);
  uv_gsource_free(app->source);
  uv_loop_close(app->loop);
  free(app);
}

static inline void execute_next_tick_listeners(BarApp* app) {
  ASSERT(app);
  Callback* cb = app->next_tick_listeners;
  Callback* prev = nullptr;
  app->next_tick_listeners = nullptr;
  do {
    if (!cb)
      break;
    mbar_exec_cb(app, cb);
    prev = cb;
    cb = cb->next;
    cb_free(prev);
  } while (cb);
}

void mbar_exec_cb(BarApp* app, Callback* cb) {
  ASSERT(app);
  ASSERT(cb);
  if (cb_is_lua(cb)) {
#define L app->L
    cbL_pcall(L, cb);
#undef L
  } else {
    cb_ccall(cb);
  }
}

void mbar_on_tick(BarApp* app) {
  ASSERT(app);
  execute_next_tick_listeners(app);
  // TODO(@s0cks): run next tick listeners
}
