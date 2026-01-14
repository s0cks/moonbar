#ifndef BAR_STATE_GTK_H
#define BAR_STATE_GTK_H

#include "mybar.h"

void bar_init_gtk_app(BarApp* app);
void bar_init_gtk_window(BarApp* app);
void bar_load_style(BarApp* app);

static inline void
bar_set_main_box(BarApp* app, GtkCenterBox* box) {
  ASSERT(app);
  ASSERT(box);
  app->box = box;
  //TODO(@s0cks): cleanup other box
}

static inline GtkWidget*
bar_set_left(BarApp* app, GtkWidget* w) {
  ASSERT(app);
  ASSERT(w);
  gtk_center_box_set_start_widget(app->box, w);
  app->left = w;
  return w;
}

static inline GtkWidget*
bar_set_center(BarApp* app, GtkWidget* w) {
  ASSERT(app);
  ASSERT(w);
  gtk_center_box_set_center_widget(app->box, w);
  app->center = w;
  return w;
}

static inline GtkWidget*
bar_set_right(BarApp* app, GtkWidget* w) {
  ASSERT(app);
  ASSERT(w);
  gtk_center_box_set_end_widget(app->box, w);
  app->right = w;
  return w;
}

static inline void
bar_signal_connect(BarApp* app, const char* signal, GCallback cb) {
  ASSERT(app);
  g_signal_connect(app->app, "activate", cb, app);
}

static inline GdkDisplay*
bar_get_display(BarApp* app) {
  ASSERT(app);
  ASSERT(app->window);
  return gtk_widget_get_display(app->window);
}

static inline void
bar_get_display_safely(BarApp* app, GdkDisplay** display) {
  ASSERT(app);
  ASSERT((*display) == NULL);
  (*display) = bar_get_display(app);
  if(!(*display)) {
    fprintf(stderr, "failed to get gtk display from window.\n");
    exit(1);
  }
}

#endif // BAR_STATE_GTK_H
