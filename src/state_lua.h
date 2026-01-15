#ifndef MBAR_LSTATE_H
#define MBAR_LSTATE_H

#include "moonbar.h"
#include "app.h"

#define LUA_GLOBAL_MBAR_STATE "mbar_state"

void mbarL_init(BarApp* app);
void mbarL_doinit(BarApp* app);
void mbarL_close(BarApp* app);
void mbarL_init_config_ref(BarApp* app);
void mbarL_call_config_init(BarApp* app);

static inline void
mbarL_push_config_ref(BarApp* app) {
  ASSERT(app);
  ASSERT(app->config_ref != LUA_NOREF);
#define L app->L
  lua_rawgeti(L, LUA_REGISTRYINDEX, app->config_ref);
#undef L
}

void mbarL_init_api(lua_State* L);
void mbarL_initmetatable_widget(lua_State* L);
void mbarL_initmetatable_label(lua_State* L);
void mbarL_initmetatable_button(lua_State* L);
void mbarL_initlib_button(lua_State* L);
void mbarL_initmetatable_event_route(lua_State* L);

static inline void
mbarL_hello_world(BarApp* app) {
  static const char* kHelloWorld = "print('Hello World')";
  mbarL_dostring(app, kHelloWorld);
}

#define mbarL_check_global_app(L, Name)                    \
  BarApp* Name = mbarL_get_mbar_app(L);                     \
  if(!Name) {                                             \
    luaL_error(L, "failed to get global bar state");      \
    return 0;                                             \
  }

#define DECLARE_LUA_INITLIB(Name)               \
  static const char* kLibName = #Name "Lib";    \
  static const luaL_Reg kLibFuncs[] =           \

#define DEFINE_LUA_INITLIB(Name)                \
  void mbarL_initlib_##Name(lua_State* L) {     \
    lua_newtable(L);                            \
    luaL_setfuncs(L, kLibFuncs, 0);             \
    lua_pushvalue(L, -1);                       \
    lua_setfield(L, -2, "__index");             \
    lua_setglobal(L, kLibName);                 \
  }

#endif // MBAR_LSTATE_H
