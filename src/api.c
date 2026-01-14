#include "mybar.h"
#include "state_lua.h"
#include "mybar/luabindings.h"
#include "util.h"

DEFINE_LUA_F(is_debug) {
  lua_pushboolean(L, BAR_DEBUG);
  return 1;
}

DEFINE_LUA_F(get_config_dir) {
  barL_check_global_app(L, app);
  lua_pushstring(L, app->home);
  return 1;
}

DEFINE_LUA_F(get_cwd) {
  barL_check_global_app(L, app);
  lua_pushstring(L, bar_get_cwd());
  return 1;
}

DEFINE_LUA_F(on) {
  barL_check_global_app(L, app);
  const char* event = lua_tostring(L, 1);
  event_route_insertl(app->events, event, L, 2);
  return 0;
}

static const luaL_Reg kLibFuncs[] = {
  { "is_debug", is_debug },
  { "get_config_dir", get_config_dir },
  { "get_cwd", get_cwd },
  { "on", on },
  { NULL, NULL },
};

void barL_init_api(lua_State* L) {
  lua_newtable(L);
  luaL_setfuncs(L, kLibFuncs, 0);
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");
  lua_setglobal(L, "CoreLib");

  barL_initmetatable_label(L);
  barL_initmetatable_button(L);
}
