#include "state_lua.h"
#include "util.h"
#include "app.h"

static const char* kInitFilename = "init.lua";

static inline char*
get_mbar_init_filename(BarApp* app) {
  ASSERT(app);
  size_t filename_size = strlen(app->home) + strlen(kInitFilename) + 2;
  char* filename = (char*)malloc(sizeof(char) * filename_size);
  snprintf(filename, filename_size, "%s/%s", app->home, kInitFilename);
  return filename;
}

void mbarL_doinit(BarApp* app) {
  ASSERT(app);
  char* init_filename = get_mbar_init_filename(app);
  if(!init_filename) {
    mbar_error(app, "failed to get init filename");
    return;
  }
  mbarL_dofile(app, init_filename);
  mbarL_init_config_ref(app);
  free(init_filename);
}

const char* mbarL_get_style(BarApp* app) {
  ASSERT(app);
#define L app->L
  mbarL_push_config_ref(app);
  lua_getfield(L, -1, "style");
  if(lua_isstring(L, -1)) {
    return lua_tostring(L, -1);
  } else if(!lua_isnoneornil(L, -1)) {
    mbar_error(app, "invalid style type");
    return NULL;
  }

  return "default";
#undef L
}

void mbarL_pushlabel(lua_State* L, Label* value) {
  ASSERT(L);
  ASSERT(value);
  lua_pushlightuserdata(L, value);
  luaL_getmetatable(L, kLabelMetatableName);
  lua_setmetatable(L, -2);
}

void mbarL_pusheventroute(lua_State* L, EventRoute* value) {
  ASSERT(L);
  ASSERT(value);
  lua_pushlightuserdata(L, value);
  luaL_getmetatable(L, kEventRouteMetatableName);
  lua_setmetatable(L, -2);
}

void mbarL_pushbutton(lua_State* L, Button* value) {
  ASSERT(L);
  ASSERT(value);
  lua_pushlightuserdata(L, value);
  luaL_getmetatable(L, kButtonMetatableName);
  lua_setmetatable(L, -2);
}

Button* mbarL_tobutton(lua_State* L, const int index) {
  ASSERT(L);
  ASSERT(index != 0);
  return (Button*)lua_touserdata(L, index);
}

static inline void
mbarL_init_bindings(BarApp* app) {
  ASSERT(app);
#define L app->L
  lua_pushlightuserdata(L, app);
  lua_setglobal(L, LUA_GLOBAL_MBAR_STATE);
#undef L
}

BarApp* mbarL_get_mbar_app(lua_State* L) {
  lua_getglobal(L, LUA_GLOBAL_MBAR_STATE);
  return (BarApp*)lua_touserdata(L, -1);
}

void mbarL_init(BarApp* app) {
  ASSERT(app);
#define L app->L
  L = luaL_newstate();
  if(!L) {
    mbar_error(app, "failed to create lua state");
    return;
  }

  luaL_openlibs(L);
  mbarL_init_bindings(app);
  mbarL_init_api(L);
  mbarL_doinit(app);
#undef L
}

void mbarL_close(BarApp* app) {
  ASSERT(app);
#define L app->L
  if(L)
    lua_close(L);
#undef L
}

void mbarL_dostring(BarApp* app, const char* code) {
  ASSERT(app);
  ASSERT(code);
#define L app->L
  if(luaL_dostring(L, code) != LUA_OK) {
    fprintf(stderr, "error executing lua: %s\n", lua_tostring(L, -1));
    return;
  }
#undef L
}

void mbarL_dofile(BarApp* app, const char* filename) {
  ASSERT(app);
  ASSERT(filename);

  if(!file_exists(filename)) {
    fprintf(stderr, "bar failed to exec lua, cannot find file: %s\n", filename);
    return;
  }

#define L app->L
  int status = luaL_loadfile(L, filename);
  status = lua_pcall(L, 0, 1, 0);
  if(status != LUA_OK) {
    fprintf(stderr, "failed to execute %s: %s\n", filename, lua_tostring(L, -1));
  }
#undef L
}

void mbarL_init_config_ref(BarApp* app) {
  ASSERT(app);
#define L app->L
  if(!lua_istable(L, -1)) {
    mbar_error(app, "expected config to return a table");
    return;
  }
  app->config_ref = luaL_ref(L, LUA_REGISTRYINDEX);
#undef L
}

void mbarL_call_config_init(BarApp* app) {
  ASSERT(app);
  mbarL_push_config_ref(app);
#define L app->L
  lua_getfield(L, -1, "init");
  if(lua_isnoneornil(L, -1)) {
    DLOG("config config['init'] is nil");
    return;
  } else if(!lua_isfunction(L, -1)) {
    DLOG("config['init'] is not a function");
    return;
  }
  int status = lua_pcall(L, 0, 1, 0);
  if(status != LUA_OK) {
    mbar_error(app, "failed to executing config[init]");
    return;
  }
#undef L
}
