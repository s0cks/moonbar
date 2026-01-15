#include "moonbar.h"
#include "button.h"
#include "state_lua.h"

DEFINE_LUA_F(button_new) {
  const char* text = NULL;
  if(lua_isnoneornil(L, -1)) {
    text = NULL;
  } else if(lua_isstring(L, -1)) {
    text = lua_tostring(L, -1);
  } else {
    text = lua_tostring(L, -1);
  }

  BarApp* app = mbarL_get_mbar_app(L);
  if(!app) {
    luaL_error(L, "failed to get global bar state");
    return 0;
  }

  Button* value = mbar_create_button(app, text);
  if(!value) {
    luaL_error(L, "failed to create gtk button.");
    return 0;
  }
  mbarL_pushbutton(L, value);
  return 1;
}

DECLARE_LUA_INITLIB(Button) {
  { "new", button_new },
  { NULL, NULL },
};

DEFINE_LUA_INITLIB(button);
