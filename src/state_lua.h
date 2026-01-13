#ifndef BAR_LSTATE_H
#define BAR_LSTATE_H

#include "mybar.h"

#define LUA_GLOBAL_BAR_STATE "bar_state"

void barL_init(BarState* state);
void barL_doinit(BarState* state);
void barL_close(BarState* state);

void barL_initmetatable_evr(lua_State* L);
void barL_initmetatable_label(lua_State* L);
void barL_initmetatable_button(lua_State* L);

static inline void
barL_hello_world(BarState* state) {
  static const char* kHelloWorld = "print('Hello World')";
  barL_dostring(state, kHelloWorld);
}

#endif // BAR_LSTATE_H
