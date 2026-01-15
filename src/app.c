#include "moonbar.h"
#include "state_lua.h"
#include "state_gtk.h"
#include "uv_gsource.h"
#include "util.h"
#include "log.h"

void mbar_error(BarApp* app, const char* err) {
  ASSERT(app);
  ASSERT(err);
  fprintf(stderr, "bar state error: %s\n", err);
  exit(1);
}

void mbar_publish(BarApp* app, const char* event) {
  EventRoute* root = event_route_search(app->events, event);
  if(!root)
    return;
#define L app->L
  event_route_call(L, root);
#undef L
}

static inline void
on_tick(uv_timer_t* handle) {
  BarApp* app = (BarApp*)handle->data;
  mbar_on_tick(app);
}

static inline gboolean
on_update_tick(gpointer data) {
  BarApp* app = (BarApp*)data;
  LOG("g update tick");
  return G_SOURCE_CONTINUE;
}

static inline int
create_update_timer(BarApp* app) {
  return g_timeout_add_seconds(1, on_update_tick, app);
}

void mbar_start_update_timer(BarApp* app) {
  ASSERT(app);
  if(app->update_timer != 0)
    return;
  app->update_timer = create_update_timer(app);
}

static inline bool
is_changed(const int events) {
  return (events & UV_CHANGE);
}

static inline void
on_style_changed(uv_fs_event_t* handle, const char* filename, int events, int status) {
  BarApp* app = (BarApp*)handle->data;
  if(status < 0) {
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
  if(mbar_is_style_watcher_running(app))
    return;
  const char* filename = mbar_config_get_style(app);
  DLOG_F("starting style filewatcher for %s\n", filename);
  const int status = uv_fs_event_start(&app->style_watcher, &on_style_changed, filename, 0);
  if(status < 0)
    mbar_error(app, "failed to create style watcher");
}

bool mbar_app_init(BarApp* app, int argc, char** argv) {
  ASSERT(app);
  app->argc = argc;
  app->argv = argv;
  app->loop = uv_loop_new();
  app->home = mbar_get_config_dir();
  app->events = event_route_new();
  app->next_tick_listeners = NULL;
  app->box = NULL;

  uv_timer_init(app->loop, &app->timer);
  uv_timer_start(&app->timer, on_tick, 0, 1000);
  app->timer.data = app;

  uv_fs_event_init(app->loop, &app->style_watcher);
  app->style_watcher.data = app;

  app->source = uv_gsource_init(app->loop);
  mbarL_init(app);
  mbar_init_gtk_app(app);
  return true;
}

bool mbar_app_run(BarApp* app) {
  int status = g_application_run(G_APPLICATION(app->app), app->argc, app->argv);
  //TODO(@s0cks): check status
  return true;
}

void mbar_app_free(BarApp* app) {
  ASSERT(app);
  if(app->app)
    g_object_unref(app->app);
  mbarL_close(app);
  if(app->home)
    free(app->home);
  uv_gsource_free(app->source);
  uv_loop_close(app->loop);
  free(app);
}

static inline void
execute_next_tick_listeners(BarApp* app) {
  ASSERT(app);
  Callback* cb = app->next_tick_listeners;
  Callback* prev = NULL;
  app->next_tick_listeners = NULL;
  do {
    if(!cb)
      break;
    mbar_exec_cb(app, cb);
    prev = cb;
    cb = cb->next;
    cb_free(prev);
  } while(cb);
}

void mbar_exec_cb(BarApp* app, Callback* cb) {
  ASSERT(app);
  ASSERT(cb);
  if(cb_is_lua(cb)) {
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
  //TODO(@s0cks): run next tick listeners
}
