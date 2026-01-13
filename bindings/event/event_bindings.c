#include "event_bindings.h"

DEFINE_LUA_F(event_router_new) {
  EventRouter* events = evr_new();
  barL_pushevr(L, events);
  return 1;
}

static const luaL_Reg kEventLib[] = {
  {"new", event_router_new},
  {NULL, NULL}
};

LUALIB_API int luaopen_event_bindings(lua_State* L) {
  luaL_newlib(L, kEventLib);
  return 1;
}
