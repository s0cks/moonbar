#include "button.h"
#include "app.h"

#define DEFINE_ON_SIGNAL(Signal)                        \
  static inline void                                    \
  on_btn_##Signal(GtkWidget* widget, gpointer data) {   \
    mbar_button_publish((Button*)data, #Signal);        \
  }

  FOR_EACH_BUTTON_SIGNAL(DEFINE_ON_SIGNAL)
#undef DEFINE_ON_SIGNAL

Button* mbar_create_button(BarApp* app, const char* text) {
  ASSERT(app);
  GtkWidget* widget = text != NULL ? gtk_button_new_with_label(text) : gtk_button_new();
  if(!widget) {
    mbar_error(app, "failed to create Label");
    return NULL;
  }
  Button* value = (Button*)malloc(sizeof(Button));
  value->owner = app;
  value->handle = widget;
  value->events = event_route_new();
#define CONNECT_SIGNAL(Signal) \
  g_signal_connect(value->handle, #Signal, G_CALLBACK(on_btn_##Signal), value);
  FOR_EACH_BUTTON_SIGNAL(CONNECT_SIGNAL)
#undef CONNECT_SIGNAL
  g_object_ref(value->handle);
  return value;
}

void mbar_button_publish(Button* btn, const char* event) {
  ASSERT(btn);
  EventRoute* root = event_route_search(btn->events, event);
  if(!root)
    return;
#define L btn->owner->L
  event_route_call(L, root);
#undef L
}

void mbar_free_button(Button* btn) {
  if(!btn)
    return;
  event_route_free(btn->owner->L, btn->events);
  g_object_unref(btn->handle);
  free(btn);
}
