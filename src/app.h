#ifndef BAR_APP_H
#define BAR_APP_H

#include "log.h"
#include "mybar.h"

void bar_error(BarApp* app, const char* error);
void bar_on_tick(BarApp* app);
void bar_publish(BarApp* app, const char* event);
void bar_add_left(BarApp* app, GtkWidget* widget);
void bar_exec_cb(BarApp* app, Callback* cb);

#endif // BAR_APP_H
