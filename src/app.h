#ifndef MBAR_APP_H
#define MBAR_APP_H

#include "log.h"
#include "moonbar.h"

void mbar_error(BarApp* app, const char* error);
void mbar_on_tick(BarApp* app);
void mbar_publish(BarApp* app, const char* event);
void mbar_add_left(BarApp* app, GtkWidget* widget);
void mbar_exec_cb(BarApp* app, Callback* cb);

#endif // MBAR_APP_H
