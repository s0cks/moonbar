#ifndef MBAR_LSTATE_H
#define MBAR_LSTATE_H

#include "moonbar.h"
#include "app.h"
#include "moonbar/luabindings.h"

#define LUA_GLOBAL_MBAR_STATE "mbar_state"

void mbarL_init(BarApp* app);
void mbarL_doinit(BarApp* app);
void mbarL_close(BarApp* app);
void mbarL_init_config_ref(BarApp* app);
const char* mbarL_get_style(BarApp* app);
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
void mbarL_initmetatable_label(lua_State* L);
void mbarL_initmetatable_button(lua_State* L);
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

#endif // MBAR_LSTATE_H
