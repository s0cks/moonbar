#include "mybar.h"
#include "state_lua.h"
#include "state_gtk.h"
#include "uv_gsource.h"
#include "util.h"
#include "log.h"

void bar_error(BarApp* app, const char* err) {
  ASSERT(app);
  ASSERT(err);
  fprintf(stderr, "bar state error: %s\n", err);
  exit(1);
}

void bar_publish(BarApp* app, const char* event) {
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
  bar_on_tick(app);
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

void bar_start_update_timer(BarApp* app) {
  ASSERT(app);
  if(app->update_timer != 0)
    return;
  app->update_timer = create_update_timer(app);
}

bool bar_app_init(BarApp* app, int argc, char** argv) {
  ASSERT(app);
  app->argc = argc;
  app->argv = argv;
  app->loop = uv_loop_new();
  app->home = bar_get_config_dir();
  app->events = event_route_new();
  app->next_tick_listeners = NULL;
  uv_timer_init(app->loop, &app->timer);
  uv_timer_start(&app->timer, on_tick, 0, 1000);
  app->timer.data = app;
  app->source = uv_gsource_init(app->loop);
  barL_init(app);
  bar_init_gtk_app(app);
  return true;
}

bool bar_app_run(BarApp* app) {
  int status = g_application_run(G_APPLICATION(app->app), app->argc, app->argv);
  //TODO(@s0cks): check status
  return true;
}

void bar_app_free(BarApp* app) {
  ASSERT(app);
  if(app->app)
    g_object_unref(app->app);
  barL_close(app);
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
    bar_exec_cb(app, cb);
    prev = cb;
    cb = cb->next;
    cb_free(prev);
  } while(cb);
}

void bar_exec_cb(BarApp* app, Callback* cb) {
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

void bar_on_tick(BarApp* app) {
  ASSERT(app);
  execute_next_tick_listeners(app);
  //TODO(@s0cks): run next tick listeners
}
