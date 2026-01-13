#include "state_gtk.h"
#include "state_lua.h"

static inline gboolean
on_idle(gpointer data) {
  BarState* state = (BarState*)data;
  bar_publish(state, "idle");
  return G_SOURCE_CONTINUE;
}

static inline void
on_activate(GtkApplication* app, gpointer user_data) {
  BarState* state = (BarState*)user_data;
  bar_init_gtk_window(state);

  barL_doinit(state);

  g_idle_add((GSourceFunc) on_idle, user_data);
  // GTK4 windows are hidden by default
  gtk_widget_show(state->window);
}


void bar_init_gtk_app(BarState* state) {
  ASSERT(state);
  GtkApplication* app = gtk_application_new("org.example.gtk4bar", G_APPLICATION_DEFAULT_FLAGS);
  if(!app) {
    bar_state_error(state, "failed to create gtk application");
    return;
  }

  state->app = app;
  bar_signal_connect(state, "activate", G_CALLBACK(on_activate));
}

void bar_init_gtk_window(BarState* state) {
  ASSERT(state);
  GtkWidget* window = gtk_application_window_new(state->app);
  if(!window) {
    bar_state_error(state, "failed to create gtk window");
    return;
  }
  gtk_layer_init_for_window(GTK_WINDOW(window));
  gtk_layer_set_layer(GTK_WINDOW(window), GTK_LAYER_SHELL_LAYER_TOP);
  gtk_layer_auto_exclusive_zone_enable(GTK_WINDOW(window));
  gtk_layer_set_anchor(GTK_WINDOW(window), GTK_LAYER_SHELL_EDGE_TOP, TRUE);
  gtk_layer_set_anchor(GTK_WINDOW(window), GTK_LAYER_SHELL_EDGE_LEFT, TRUE);
  gtk_layer_set_anchor(GTK_WINDOW(window), GTK_LAYER_SHELL_EDGE_RIGHT, TRUE);
  state->window = window;
}

void bar_load_style(BarState* state, const char* filename) {
  ASSERT(state);
  ASSERT(filename);
#ifdef BAR_DEBUG
  fprintf(stdout, "loading bar style from %s .....\n", filename);
#endif // BAR_DEBUG
  //
  GtkCssProvider* css_provider = gtk_css_provider_new();
  if(!css_provider) {
    fprintf(stderr, "failed to create gtk css provider.\n");
    exit(1);
  }

  GdkDisplay* display = NULL;
  bar_get_display_safely(state, &display);

  GFile* css_file = g_file_new_for_path(filename);
  gtk_css_provider_load_from_file(css_provider, css_file);
  gtk_style_context_add_provider_for_display(display, GTK_STYLE_PROVIDER(css_provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

  g_object_unref(css_file);
  g_object_unref(css_provider);
#ifdef BAR_DEBUG

  fprintf(stdout, "finished loading bar style.\n");

#endif // BAR_DEBUG
}
