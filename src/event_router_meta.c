#include "moonbar/event_router.h"
#include "moonbar.h"

DEFINE_LUA_F(on) {
  EventRoute* route = (EventRoute*)lua_touserdata(L, 1);
  if(!route) {
    luaL_error(L, "invalid EventRoute userdata");
    return 0;
  }
  const char* event = lua_tostring(L, 2);
  //TODO(@s0cks): get function reference
  return 1;
}

DEFINE_LUA_F(publish) {
  EventRoute* route = (EventRoute*)lua_touserdata(L, 1);
  if(!route) {
    luaL_error(L, "invalid EventRoute userdata");
    return 0;
  }
  const char* event = lua_tostring(L, -1);
  return 1;
}

static const luaL_Reg kEventRouteFuncs[] = {
  { "on", on },
  { "publish", publish },
  {NULL, NULL},
};

DEFINE_LUA_F(new_event_route) {
  EventRoute* value = event_route_new();
  mbarL_pusheventroute(L, value);
  return 1;
}

static const luaL_Reg kEventRouteLibFuncs[] = {
  { "new", new_event_route },
  { NULL, NULL },
};

void mbarL_initmetatable_event_route(lua_State* L) {
  luaL_newmetatable(L, kEventRouteMetatableName);
  luaL_setfuncs(L, kEventRouteFuncs, 0);
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");

  lua_newtable(L);
  luaL_setfuncs(L, kEventRouteLibFuncs, 0);
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");
  lua_setglobal(L, "EventRouteLib");
}
