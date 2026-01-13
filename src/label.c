#include "label.h"
#include "state.h"

Label* bar_create_label(BarState* state, const char* text) {
  GtkWidget* widget = gtk_label_new(text);
  if(!widget) {
    bar_state_error(state, "failed to create Label");
    return NULL;
  }
  gtk_window_set_child(GTK_WINDOW(state->window), widget);
  Label* label = (Label*)malloc(sizeof(Label));
  label->owner = state;
  label->handle = widget;
  memset(&label->callbacks[0], 0, sizeof(label->callbacks));
  g_object_ref(label->handle);
  return label;
}

void bar_free_label(Label* value) {
  if(!value)
    return;
  //TODO(@s0cks): should probably free callbacks in Label
  g_object_unref(value->handle);
  free(value);
}
