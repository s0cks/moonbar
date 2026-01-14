#ifndef BAR_LSTATE_H
#define BAR_LSTATE_H

#include "mybar.h"
#include "mybar/luabindings.h"

#define LUA_GLOBAL_BAR_STATE "bar_state"

void barL_init(BarApp* app);
void barL_doinit(BarApp* app);
void barL_close(BarApp* app);
void barL_init_config_ref(BarApp* app);
const char* barL_get_style(BarApp* app);
void barL_call_config_init(BarApp* app);

static inline void
barL_push_config_ref(BarApp* app) {
  ASSERT(app);
  ASSERT(app->config_ref != LUA_NOREF);
#define L app->L
  lua_rawgeti(L, LUA_REGISTRYINDEX, app->config_ref);
#undef L
}

void barL_init_api(lua_State* L);
void barL_initmetatable_label(lua_State* L);
void barL_initmetatable_button(lua_State* L);

static inline void
barL_hello_world(BarApp* app) {
  static const char* kHelloWorld = "print('Hello World')";
  barL_dostring(app, kHelloWorld);
}

#define barL_check_global_app(L, Name)                    \
  BarApp* Name = barL_get_bar_app(L);                     \
  if(!Name) {                                             \
    luaL_error(L, "failed to get global bar state");      \
    return 0;                                             \
  }

#endif // BAR_LSTATE_H
