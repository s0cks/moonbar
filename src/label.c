#include "label.h"
#include "app.h"

Label* bar_create_label(BarApp* app, const char* text) {
  GtkWidget* widget = gtk_label_new(text);
  if(!widget) {
    bar_error(app, "failed to create Label");
    return NULL;
  }
  Label* label = (Label*)malloc(sizeof(Label));
  label->owner = app;
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
