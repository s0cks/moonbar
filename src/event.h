#ifndef BAR_EVENT_H
#define BAR_EVENT_H

#include "mybar.h"

typedef struct _EventRoute {
  struct _EventRoute* next;
  int callback;
} EventRoute;

static inline void
route_init(lua_State* L, EventRoute* route, const int index) {
  luaL_checktype(L, 1, LUA_TFUNCTION);
  lua_pushvalue(L, index);
  route->callback = luaL_ref(L, LUA_REGISTRYINDEX);
}

static inline bool
route_has_callback(EventRoute* route) {
  ASSERT(route);
  return route->callback != LUA_NOREF;
}

static inline void
route_unref_callback(lua_State* L, EventRoute* route) {
  ASSERT(L);
  ASSERT(route);
  ASSERT(route_has_callback(route));
  luaL_unref(L, LUA_REGISTRYINDEX, route->callback);
  route->callback = LUA_NOREF;
}

static constexpr const int kEventRouterAlphabetSize = 32;
struct _EventRouter {
  EventRouter* children[kEventRouterAlphabetSize];
  EventRoute route;
  bool terminal : 1;
};

#endif // BAR_EVENT_H
