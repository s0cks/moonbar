#ifndef MBAR_STATE_GTK_H
#define MBAR_STATE_GTK_H

#include "moonbar.h"
#include "app.h"

void mbar_init_gtk_app(BarApp* app);
void mbar_init_gtk_window(BarApp* app);
void mbar_load_style(BarApp* app);

static inline void
mbar_set_main_box(BarApp* app, GtkCenterBox* box) {
  ASSERT(app);
  ASSERT(box);
  app->box = box;
  //TODO(@s0cks): cleanup other box
}

static inline GtkWidget*
mbar_set_left(BarApp* app, GtkWidget* w) {
  ASSERT(app);
  ASSERT(w);
  gtk_center_box_set_start_widget(app->box, w);
  app->left = w;
  return w;
}

static inline GtkWidget*
mbar_set_center(BarApp* app, GtkWidget* w) {
  ASSERT(app);
  ASSERT(w);
  gtk_center_box_set_center_widget(app->box, w);
  app->center = w;
  return w;
}

static inline GtkWidget*
mbar_set_right(BarApp* app, GtkWidget* w) {
  ASSERT(app);
  ASSERT(w);
  gtk_center_box_set_end_widget(app->box, w);
  app->right = w;
  return w;
}

static inline void
mbar_signal_connect(BarApp* app, const char* signal, GCallback cb) {
  ASSERT(app);
  g_signal_connect(app->app, "activate", cb, app);
}

static inline GdkDisplay*
mbar_get_display(BarApp* app) {
  ASSERT(app);
  ASSERT(app->window);
  return gtk_widget_get_display(app->window);
}

static inline void
mbar_get_display_safely(BarApp* app, GdkDisplay** display) {
  ASSERT(app);
  ASSERT((*display) == NULL);
  (*display) = mbar_get_display(app);
  if(!(*display))
    mbar_error(app, "failed to get gtk display from window");
}

#endif // MBAR_STATE_GTK_H
