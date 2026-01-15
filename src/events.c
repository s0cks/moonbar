#include <stdlib.h>
#include <string.h>
#include "moonbar.h"

EventRoute* event_route_new() {
  EventRoute* node = (EventRoute*)malloc(sizeof(EventRoute));
  if(!node)
    return NULL;
  node->path = NULL;
  node->terminal = false;
  node->callback = LUA_NOREF;
  memset(node->children, 0, sizeof(node->children));
  return node;
}

bool event_route_call(lua_State* L, EventRoute* root) {
  if(!root || root->callback == LUA_NOREF)
    return false;
  lua_rawgeti(L, LUA_REGISTRYINDEX, root->callback);
  return lua_pcall(L, 0, 0, 0) == LUA_OK;
}

//TODO(@s0cks): remove L param or store L in EventRoute
void event_route_free(lua_State* L, EventRoute* root) {
  if(!root)
    return;
  if(root->callback != LUA_NOREF)
    luaL_unref(L, LUA_REGISTRYINDEX, root->callback);
  for(int idx = 0; idx < kEventAlphabetSize; idx++) {
    if(!root->children[idx])
      continue;
    event_route_free(L, root->children[idx]);
  }
  free(root);
}

static inline int to_index(const char c) {
  return c - 'a';
}

#define BEGIN_FOREACH_CHAR_IN_EVENT(EventVar, IndexVar) \
  for(int i = 0; EventVar[i] != '\0'; i++) {                 \
    int IndexVar = to_index(EventVar[i]);                           \

#define END_FOREACH_CHAR_IN_EVENT }

EventRoute* event_route_insert(EventRoute* root, const char* event, const int callback) {
  EventRoute* curr = root;
  BEGIN_FOREACH_CHAR_IN_EVENT(event, index)
    if(curr->children[index] == NULL)
      curr->children[index] = event_route_new();
    curr = curr->children[index];
  END_FOREACH_CHAR_IN_EVENT
  curr->path = strdup(event);
  curr->callback = callback;
  curr->terminal = true;
  return curr;
}

EventRoute* event_route_insertl(EventRoute* route, const char* event, lua_State* L, const int index) {
  //TODO(@s0cks): check if index is function
  lua_pushvalue(L, index);
  int callback = luaL_ref(L, LUA_REGISTRYINDEX);
  return event_route_insert(route, event, callback);
}

EventRoute* event_route_search(EventRoute* root, const char* event) {
  EventRoute* curr = root;
  BEGIN_FOREACH_CHAR_IN_EVENT(event, index)
    if(curr->children[index] == NULL)
      return NULL;
    curr = curr->children[index];
  END_FOREACH_CHAR_IN_EVENT
  return curr && curr->terminal ? curr : NULL;
}
