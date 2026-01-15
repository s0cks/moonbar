#include "moonbar.h"
#include "state_lua.h"

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

DECLARE_LUA_METATABLE(EventRoute) {
  { "on", on },
  { "publish", publish },
  {NULL, NULL},
};

DEFINE_LUA_INITMETATABLE(event_route, EventRoute);
