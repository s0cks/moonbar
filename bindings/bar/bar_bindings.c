#include "mbar_bindings.h"

#define FOR_EACH_LIB_FUNC(V) \
  V(get_home) \
  V(is_debug) \
  V(style)

DECLARE_LUAOPEN_ENTRY_FUNCS(bar, FOR_EACH_LIB_FUNC);

DEFINE_LUA_F(get_home) {
  BarState* state = mbarL_get_mbar_state(L);
  lua_pushstring(L, state->home);
  return 1;
}

DEFINE_LUA_F(is_debug) {
  lua_pushboolean(L, (bool)MBAR_DEBUG);
  return 1;
}

DEFINE_LUA_F(style) {
  const char* filename = lua_tostring(L, -1);
  BarState* state = mbarL_get_mbar_state(L);
  mbar_load_style(state, filename);
  return 0;
}

DEFINE_LUAOPEN_ENTRY(bar);
