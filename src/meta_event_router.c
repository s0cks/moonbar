#include "mybar.h"
#include "state_lua.h"
#include "mybar/luabindings.h"

#define EventRouterFromUserdata(Name, index)                                  \
  EventRouter* Name = barL_getevr(L, index);                                  \
  if(!Name) {                                                                 \
    luaL_error(L, "failed to get %s userdata", kEventRouterMetatableName);    \
    return 0;                                                                 \
  }

DEFINE_LUA_F(event_router_on) {
  EventRouterFromUserdata(evr, 1);
  const char* event = lua_tostring(L, 2);
  fprintf(stdout, "register function for event '%s'\n", event);
  return 0;
}

DEFINE_LUA_F(event_router_publish) {
  EventRouterFromUserdata(evr, 1);
  const char* event = lua_tostring(L, 2);
  fprintf(stdout, "publishing event '%s'\n", event);
  return 0;
}

static const luaL_Reg kEventRouterFuncs[] = {
  {"on", event_router_on},
  {"publish", event_router_publish},
  {NULL, NULL},
};

void barL_initmetatable_evr(lua_State* L) {
  luaL_newmetatable(L, kEventRouterMetatableName);

  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");
  luaL_register(L, NULL, kEventRouterFuncs);
}
