#ifndef BAR_STATE_GTK_H
#define BAR_STATE_GTK_H

#include "mybar.h"
#include "state.h"

void bar_init_gtk_app(BarApp* app);
void bar_init_gtk_window(BarApp* app);

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
