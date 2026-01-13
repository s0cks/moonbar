#include "label_bindings.h"
#include "mybar.h"
#include "mybar/luabindings.h"

DEFINE_LUA_F(new_label) {
  const char* text = NULL;
  if(lua_isnoneornil(L, -1)) {
    text = "";
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

  Label* label = bar_create_label(state, text);
  if(!label) {
    luaL_error(L, "failed to create gtk label");
    return 0;
  }
  barL_pushlabel(L, label);
  return 1;
}

static const luaL_Reg kLabelLib[] = {
  {"new", new_label},
  {NULL, NULL}
};

LUALIB_API int luaopen_label_bindings(lua_State* L) {
  luaL_newlib(L, kLabelLib);
  return 1;
}
