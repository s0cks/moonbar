#ifndef MBAR_BUTTON_H
#define MBAR_BUTTON_H

#include "moonbar.h"

#define FOR_EACH_BUTTON_SIGNAL(V) \
  V(clicked) \
  V(show) \
  V(hide)

struct _Button {
  GtkWidget* handle;
  BarApp* owner;
  EventRoute* events;
};

void mbar_button_publish(Button* btn, const char* event);

#endif // MBAR_BUTTON_H
