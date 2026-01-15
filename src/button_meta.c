#include "moonbar.h"
#include "button.h"
#include "state_lua.h"

DEFINE_LUA_F(set_text) {
  Button* button = (Button*)lua_touserdata(L, 1);
  if(button == NULL) {
    luaL_error(L, "invalid button userdata");
    return 0;
  }
  const char* text = lua_tostring(L, -1);
  gtk_button_set_label(GTK_BUTTON(button->handle), text);
  return 1;
}

DEFINE_LUA_F(get_text) {
  Button* button = (Button*)lua_touserdata(L, 1);
  if(button == NULL) {
    luaL_error(L, "invalid button userdata");
    return 0;
  }
  const char* text = gtk_button_get_label(GTK_BUTTON(button->handle));
  lua_pushstring(L, text);
  return 1;
}

DECLARE_LUA_METATABLE(Button) {
  { "get_text", get_text },
  { "set_text", set_text },
  { NULL, NULL },
};

DEFINE_LUA_INITWIDGETMETATABLE(button, Button);
