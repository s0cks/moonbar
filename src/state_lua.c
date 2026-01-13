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
  char* init_filename = get_bar_init_filename(state);
  if(!init_filename)
    return;
  barL_dofile(state, init_filename);
  free(init_filename);
}

static inline int
new_label(lua_State* L) {
  lua_getglobal(L, "bar_state");
  BarState* state = (BarState*)lua_touserdata(L, -1);
  ASSERT(state);
  // GtkWidget *label = gtk_label_new("Arch Linux 2026 | GTK4 Status Bar");
  // gtk_window_set_child(GTK_WINDOW(window), label);
}

static inline void
barL_init_bindings(BarState* state) {
  ASSERT(state);
#define L state->L
  lua_pushlightuserdata(L, state);
  lua_setglobal(L, "bar_state");

  lua_pushcfunction(L, new_label);
  lua_setglobal(L, "new-label");
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
