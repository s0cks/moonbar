#ifndef MBAR_APP_H
#define MBAR_APP_H

#include "log.h"
#include "moonbar.h"

void mbar_error(BarApp* app, const char* error);
void mbar_on_tick(BarApp* app);
void mbar_publish(BarApp* app, const char* event);
void mbar_exec_cb(BarApp* app, Callback* cb);
void mbar_start_style_watcher(BarApp* app);

#endif // MBAR_APP_H
