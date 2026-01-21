#include "moonbar.h"
#include "state_lua.h"

static inline EventRoute* check_event_route(lua_State* L, const int index) {
  ASSERT(L);
  return (EventRoute*)luaL_checkudata(L, index, kEventRouteMetatableName);
}

DEFINE_LUA_F(on) {
  EventRoute* root = check_event_route(L, 1);
  const char* event = lua_tostring(L, 2);
  luaL_checktype(L, 3, LUA_TFUNCTION);
  event_route_insertl(root, event, L, 3);
  return 0;
}

DEFINE_LUA_F(publish) {
  EventRoute* root = check_event_route(L, 1);
  const char* event = lua_tostring(L, 2);
  EventRoute* route = event_route_search(root, event);
  if (event)
    event_route_call(L, route);
  return 0;
}

DECLARE_LUA_METATABLE(EventRoute){
    {"on", on},
    {"publish", publish},
    {nullptr, nullptr},
};

DEFINE_LUA_INITMETATABLE(event_route, EventRoute);
