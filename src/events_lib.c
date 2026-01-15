#include "moonbar.h"
#include "state_lua.h"

DEFINE_LUA_F(new_event_route) {
  EventRoute* value = event_route_new();
  mbarL_pushevent_route(L, value);
  return 1;
}

DECLARE_LUA_LIB(EventRoute) {
  { "new", new_event_route },
  { NULL, NULL },
};
DEFINE_LUA_INITLIB(event_route);
