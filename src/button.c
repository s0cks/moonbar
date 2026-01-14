#include "button.h"
#include "state.h"

Button* bar_create_button(BarApp* app, const char* text) {
  ASSERT(app);
  GtkWidget* widget = text != NULL ? gtk_button_new_with_label(text) : gtk_button_new();
  if(!widget) {
    bar_error(app, "failed to create Label");
    return NULL;
  }
  gtk_window_set_child(GTK_WINDOW(app->window), widget);
  Button* value = (Button*)malloc(sizeof(Button));
  value->owner = app;
  value->handle = widget;
  memset(&value->callbacks[0], 0, sizeof(value->callbacks));
  g_object_ref(value->handle);
  return value;
}

void bar_free_button(Button* value) {
  if(!value)
    return;
  //TODO(@s0cks): unref callbacks
  g_object_unref(value->handle);
  free(value);
}
