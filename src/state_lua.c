#include "state_lua.h"
#include "util.h"

static const char* kInitFilename = "init.lua";

static inline char*
get_bar_init_filename(BarState* state) {
  ASSERT(state);

  size_t filename_size = strlen(state->home) + strlen(kInitFilename) + 2;
  char* filename = (char*)malloc(sizeof(char) * filename_size);
  snprintf(filename, filename_size, "%s/%s", state->home, kInitFilename);
  return filename;
}

void barL_doinit(BarState* state) {
  ASSERT(state);
#define L state->L
  barL_initmetatable_evr(L);
  barL_initmetatable_label(L);
  barL_initmetatable_button(L);
#undef L
  char* init_filename = get_bar_init_filename(state);
  if(!init_filename)
    return;
  barL_dofile(state, init_filename);
  free(init_filename);
}

void barL_pushevr(lua_State* L, EventRouter* evr) {
  ASSERT(L);
  ASSERT(evr);
  lua_pushlightuserdata(L, evr);
  luaL_getmetatable(L, kEventRouterMetatableName);
  lua_setmetatable(L, -2);
}

void barL_pushlabel(lua_State* L, Label* value) {
  ASSERT(L);
  ASSERT(value);
  lua_pushlightuserdata(L, value);
  luaL_getmetatable(L, kLabelMetatableName);
  lua_setmetatable(L, -2);
}

void barL_pushbutton(lua_State* L, Button* value) {
  ASSERT(L);
  ASSERT(value);
  lua_pushlightuserdata(L, value);
  luaL_getmetatable(L, kButtonMetatableName);
  lua_setmetatable(L, -2);
}

Button* barL_tobutton(lua_State* L, const int index) {
  ASSERT(L);
  ASSERT(index != 0);
  return (Button*)lua_touserdata(L, index);
}

EventRouter* barL_getevr(lua_State* L, const int index) {
  ASSERT(L);
  ASSERT(index != 0);
  return (EventRouter*)lua_touserdata(L, index);
}

static inline void
barL_init_bindings(BarState* state) {
  ASSERT(state);
#define L state->L
  lua_pushlightuserdata(L, state);
  lua_setglobal(L, "bar_state");
#undef L
}

BarState* barL_get_bar_state(lua_State* L) {
  lua_getglobal(L, LUA_GLOBAL_BAR_STATE);
  return (BarState*)lua_touserdata(L, -1);
}

void barL_init(BarState* state) {
  ASSERT(state);
#define L state->L
  L = luaL_newstate();
  if(!L) {
    fprintf(stderr, "failed to create bar lua state.\n");
    return;
  }

  luaL_openlibs(L);
  barL_init_bindings(state);
#undef L
}

void barL_close(BarState* state) {
  ASSERT(state);
#define L state->L
  if(L)
    lua_close(L);
#undef L
}

void barL_dostring(BarState* state, const char* code) {
  ASSERT(state);
  ASSERT(code);
#define L state->L
  if(luaL_dostring(L, code) != LUA_OK) {
    fprintf(stderr, "error executing lua: %s\n", lua_tostring(L, -1));
    return;
  }
#undef L
}

void barL_dofile(BarState* state, const char* filename) {
  ASSERT(state);
  ASSERT(filename);

  if(!file_exists(filename)) {
    fprintf(stderr, "bar failed to exec lua, cannot find file: %s\n", filename);
    return;
  }

#define L state->L
  int status = luaL_loadfile(L, filename);
  status = lua_pcall(L, 0, 0, 0);
  if(status != LUA_OK) {
    fprintf(stderr, "failed to execute %s: %s\n", filename, lua_tostring(L, -1));
  }
#undef L
}
