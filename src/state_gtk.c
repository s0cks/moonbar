#include "state_gtk.h"
#include "state_lua.h"
#include "app.h"

static inline void
on_activate(GtkApplication* gtk_app, gpointer user_data) {
  BarApp* app = (BarApp*)user_data;
  bar_init_gtk_window(app);

  const char* style = barL_get_style(app);
  //TODO(@s0cks): load style

  barL_call_config_init(app);

  EventRoute* root = event_route_search(app->events, "post-init");
  if(root) {
#define L app->L
    event_route_call(L, root);
#undef L
  } else {
    fprintf(stdout, "failed to find post-init event\n");
  }
  gtk_window_present(GTK_WINDOW(app->window));
}

void bar_init_gtk_app(BarApp* app) {
  ASSERT(app);
  GtkApplication* gtk_app = gtk_application_new("org.example.gtk4bar", G_APPLICATION_DEFAULT_FLAGS);
  if(!gtk_app) {
    bar_error(app, "failed to create gtk application");
    return;
  }
  app->app = gtk_app;
  bar_signal_connect(app, "activate", G_CALLBACK(on_activate));
}

void bar_init_gtk_window(BarApp* app) {
  ASSERT(app);
  GtkWidget* window = gtk_application_window_new(app->app);
  if(!window) {
    bar_error(app, "failed to create gtk window");
    return;
  }
  gtk_layer_init_for_window(GTK_WINDOW(window));
  gtk_layer_set_layer(GTK_WINDOW(window), GTK_LAYER_SHELL_LAYER_TOP);
  gtk_layer_auto_exclusive_zone_enable(GTK_WINDOW(window));
  gtk_layer_set_anchor(GTK_WINDOW(window), GTK_LAYER_SHELL_EDGE_TOP, TRUE);
  gtk_layer_set_anchor(GTK_WINDOW(window), GTK_LAYER_SHELL_EDGE_LEFT, TRUE);
  gtk_layer_set_anchor(GTK_WINDOW(window), GTK_LAYER_SHELL_EDGE_RIGHT, TRUE);
  app->window = window;
}

void bar_load_style(BarApp* app, const char* filename) {
  ASSERT(app);
  ASSERT(filename);
  GtkCssProvider* css_provider = gtk_css_provider_new();
  if(!css_provider) {
    fprintf(stderr, "failed to create gtk css provider.\n");
    exit(1);
  }

  GdkDisplay* display = NULL;
  bar_get_display_safely(app, &display);

  GFile* css_file = g_file_new_for_path(filename);
  gtk_css_provider_load_from_file(css_provider, css_file);
  gtk_style_context_add_provider_for_display(display, GTK_STYLE_PROVIDER(css_provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

  g_object_unref(css_file);
  g_object_unref(css_provider);
}
