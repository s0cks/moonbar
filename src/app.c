#include "mybar.h"
#include "state_lua.h"
#include "state_gtk.h"
#include "uv_gsource.h"

void bar_error(BarApp* app, const char* err) {
  ASSERT(app);
  ASSERT(err);
  fprintf(stderr, "bar state error: %s\n", err);
  exit(1);
}

bool bar_app_init(BarApp* app, int argc, char** argv) {
  ASSERT(app);
  app->argc = argc;
  app->argv = argv;
  app->loop = uv_loop_new();
  app->home = bar_get_home_from_env();
  // app->source = uv_gsource_init(app->loop);
  barL_init(app);
  bar_init_gtk_app(app);
  return true;
}

bool bar_app_run(BarApp* app) {
  fprintf(stdout, "running app....\n");
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
