#ifndef BAR_EVENT_ROUTER_H
#define BAR_EVENT_ROUTER_H

#include "mybar/luabindings.h"

static constexpr const int kEventAlphabetSize = 28;
typedef struct _EventRoute EventRoute;
typedef EventRoute* EventRouteChildren[kEventAlphabetSize];
struct _EventRoute {
  EventRouteChildren children;
  char* path;
  int callback;
  bool terminal : 1;
};

EventRoute* event_route_new();
bool event_route_call(lua_State* L, EventRoute* root);
void event_route_free(lua_State* L, EventRoute* root);
EventRoute* event_route_insert(EventRoute* root, const char* event, const int callback);
EventRoute* event_route_insertl(EventRoute* route, const char* event, lua_State* L, const int index);
EventRoute* event_route_search(EventRoute* root, const char* event);

#endif // BAR_EVENT_ROUTER_H
