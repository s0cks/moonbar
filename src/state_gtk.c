#include "state_gtk.h"
#include "state_lua.h"
#include "app.h"
#include "uv_gsource.h"

// Left
static inline GtkWidget*
bar_create_left_box_with_padding(BarApp* app, const int padding) {
  GtkWidget* box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, padding);
  gtk_widget_add_css_class(box, "left");
  gtk_widget_set_halign(box, GTK_ALIGN_START);
  gtk_widget_set_valign(box, GTK_ALIGN_CENTER);
  return box;
}

static inline GtkWidget*
bar_create_left_box(BarApp* app) {
  return bar_create_left_box_with_padding(app, 0);
}

// Center
static inline GtkWidget*
bar_create_center_box_with_padding(BarApp* app, const int padding) {
  GtkWidget* box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, padding);
  gtk_widget_add_css_class(box, "center");
  gtk_widget_set_halign(box, GTK_ALIGN_CENTER);
  gtk_widget_set_valign(box, GTK_ALIGN_CENTER);
  return box;
}

static inline GtkWidget*
bar_create_center_box(BarApp* app) {
  return bar_create_center_box_with_padding(app, 0);
}

// Right
static inline GtkWidget*
bar_create_right_box_with_padding(BarApp* app, const int padding) {
  GtkWidget* box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, padding);
  gtk_widget_add_css_class(box, "right");
  gtk_widget_set_halign(box, GTK_ALIGN_END);
  gtk_widget_set_valign(box, GTK_ALIGN_CENTER);
  return box;
}

static inline GtkWidget*
bar_create_right_box(BarApp* app) {
  return bar_create_right_box_with_padding(app, 0);
}

// Main
static inline GtkCenterBox*
bar_create_main_box(BarApp* app) {
  GtkWidget* box = gtk_center_box_new();
  gtk_widget_add_css_class(box, "main");
  //TODO(@s0cks): do something?
  return GTK_CENTER_BOX(box);
}

static inline void
on_activate(GtkApplication* gtk_app, gpointer user_data) {
  BarApp* app = (BarApp*)user_data;
  barL_call_config_init(app);
  bar_publish(app, "init");
  bar_init_gtk_window(app);

  app->box = bar_create_main_box(app);
  bar_set_left(app, bar_create_left_box(app));
  bar_set_center(app, bar_create_center_box(app));
  bar_set_right(app, bar_create_right_box(app));
  gtk_window_set_child(GTK_WINDOW(app->window), GTK_WIDGET(app->box));

  bar_load_style(app);

  bar_publish(app, "post-init");
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

void bar_load_style(BarApp* app) {
  ASSERT(app);
  bar_publish(app, "style.pre-load");
  const char* filename = barL_get_style(app);
  ASSERT(filename);
#ifdef BAR_DEBUG
  fprintf(stdout, "loading style from %s ....\n", filename);
#endif // BAR_DEBUG

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
#ifdef BAR_DEBUG
  fprintf(stdout, "finished loading style.\n");
#endif // BAR_DEBUG
  bar_publish(app, "style.post-load");
}
