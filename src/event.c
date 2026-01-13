#include "event.h"

constexpr const int kBarEventAlphabetSize = 32;

void route_free(lua_State* L, EventRoute* route) {
  if(!route)
    return;
  if(route_has_callback(route))
    route_unref_callback(L, route);
  free(route);
}

EventRouter* evr_new(void) {
  EventRouter* evr = (EventRouter*)malloc(sizeof(EventRouter));
  if(evr)
    (*evr) = (EventRouter){};
  return evr;
}

void evr_free(EventRouter* evr) {
  if(!evr)
    return;
  for(int idx = 0; idx < kBarEventAlphabetSize; idx++)
    evr_free(evr->children[idx]);
  free(evr);
}

void evr_on(lua_State* L, EventRouter* evr, const char* event, const int index) {
  ASSERT(L);
  ASSERT(evr);
  ASSERT(event);
  EventRouter* curr = evr;
  for(int i = 0; event[i] != '\0'; i++) {
    int index = event[i] - 'a';
    if(!(curr->children[index]))
      curr->children[index] = evr_new();
    curr = curr->children[index];
  }
  ASSERT(curr);
  route_init(L, &curr->route, index);
  curr->terminal = true;
}

static inline bool
is_valid_route(EventRouter* evr) {
  return evr && evr->terminal && route_has_callback(&evr->route);
}

void evr_publish(lua_State* L, EventRouter* evr, const char* event) {
  ASSERT(L);
  ASSERT(evr);
  ASSERT(event);
  EventRouter* curr = evr;
  for(int i = 0; i < event[i] != '\0'; i++) {
    int index = event[i] - 'a';
    if(!curr->children[index])
      return;
    curr = curr->children[index];
  }
  if(!is_valid_route(curr))
    return;
  lua_rawgeti(L, LUA_REGISTRYINDEX, curr->route.callback);
  if(lua_pcall(L, 0, 0, 0) != LUA_OK) {
    const char* err = lua_tostring(L, -1);
    fprintf(stderr, "lua error: %s\n", err);
    exit(1);
  }
}

void evr_remove(lua_State* L, EventRouter* evr, const char* event) {
  //TODO(@s0cks): not implemented
  fprintf(stderr, "not implemented!\n");
}
