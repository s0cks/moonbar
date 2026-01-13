#ifndef BAR_STATE_GTK_H
#define BAR_STATE_GTK_H

#include "mybar.h"
#include "state.h"

void bar_init_gtk_app(BarState* state);
void bar_init_gtk_window(BarState* state);

static inline void
bar_signal_connect(BarState* state, const char* signal, GCallback cb) {
  ASSERT(state);
  g_signal_connect(state->app, "activate", cb, state);
}

static inline GdkDisplay*
bar_get_display(BarState* state) {
  ASSERT(state);
  ASSERT(state->window);
  return gtk_widget_get_display(state->window);
}

static inline void
bar_get_display_safely(BarState* state, GdkDisplay** display) {
  ASSERT(state);
  ASSERT((*display) == NULL);
  (*display) = bar_get_display(state);
  if(!(*display)) {
    fprintf(stderr, "failed to get gtk display from window.\n");
    exit(1);
  }
}

#endif // BAR_STATE_GTK_H
