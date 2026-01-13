#include "button_bindings.h"
#include "mybar.h"

DEFINE_LUA_F(button_new) {
  const char* text = NULL;
  if(lua_isnoneornil(L, -1)) {
    text = NULL;
  } else if(lua_isstring(L, -1)) {
    text = lua_tostring(L, -1);
  } else {
    text = lua_tostring(L, -1);
  }

  BarState* state = barL_get_bar_state(L);
  if(!state) {
    luaL_error(L, "failed to get global bar state");
    return 0;
  }

  Button* button = bar_create_button(state, text);
  if(!button) {
    luaL_error(L, "failed to create gtk button.");
    return 0;
  }

  barL_pushbutton(L, button);
  return 1;
}

static const luaL_Reg kButtonLib[] = {
  {"new", button_new},
  {NULL, NULL}
};

LUALIB_API int luaopen_button_bindings(lua_State* L) {
  luaL_newlib(L, kButtonLib);
  return 1;
}
