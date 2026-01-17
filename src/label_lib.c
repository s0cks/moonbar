#include "moonbar.h"
#include "label.h"
#include "state_lua.h"

DEFINE_LUA_F(new_label) {
  const char* text = NULL;
  if(lua_isnoneornil(L, -1)) {
    text = "";
  } else if(lua_isstring(L, -1)) {
    text = lua_tostring(L, 1);
  } else {
    text = lua_tostring(L, 1);
  }

  BarApp* app = mbarL_get_mbar_app(L);
  if(!app) {
    luaL_error(L, "failed to get global bar state");
    return 0;
  }

  Label* label = mbar_create_label(app, text);
  if(!label) {
    luaL_error(L, "failed to create gtk label");
    return 0;
  }
  mbarL_pushlabel(L, label);
  return 1;
}

DECLARE_LUA_LIB(Label) {
  {"new", new_label},
  {NULL, NULL}
};
DEFINE_LUA_INITLIB(label);
