#ifndef BAR_APP_H
#define BAR_APP_H

void bar_error(BarApp* app, const char* error);
void bar_publish(BarApp* app, const char* event);
void bar_add_left(BarApp* app, GtkWidget* widget);

#endif // BAR_APP_H
