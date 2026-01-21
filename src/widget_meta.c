#include "moonbar.h"
#include "state_lua.h"
#include "widget.h"

#define BUTTON_USERDATA(Name, Index)                  \
  Button* Name = (Button*)lua_touserdata(L, (Index)); \
  if (!(Name))                                        \
    INVALID_BUTTON_USERDATA;

DEFINE_LUA_F(widget_on) {
  const void* data = lua_touserdata(L, 1);
  if (!data) {
    luaL_error(L, "invalid widget for param 1");
    return 0;
  }
  const char* event = lua_tostring(L, 2);
  mbar_widget_onl(data, event, L, 3);
  return 0;
}

DEFINE_LUA_F(widget_add_class) {
  GtkWidget* widget = mbarL_check_widget(L, 1);
  const char* cls = lua_tostring(L, 2);
  gtk_widget_add_css_class(widget, cls);
  gtk_widget_queue_draw(widget);
  return 0;
}

static const luaL_Reg kWidgetFuncs[] = {
    {"on", widget_on},
    {"add_class", widget_add_class},
    {nullptr, nullptr},
};

DEFINE_LUA_INITMETATABLE(widget, Widget);
