#include "keyboard.h"

#include "upowerctl.h"

static inline int get_brightness(lua_State* L) {
  UPowerctl* ctx = (UPowerctl*)lua_touserdata(L, 1);
  if (!ctx) {
    luaL_error(L, "failed to get keyboard upowerctl context from arg #1");
    return 0;
  }
  upow_brightness_t value = 0;
  if (!upow_get(ctx, &value)) {
    luaL_error(L, "failed to get keyboard brightness from upower");
    return 0;
  }
  lua_pushnumber(L, (lua_Number)value);
  return 1;
}

static inline int set_brightness(lua_State* L) {
  UPowerctl* ctx = (UPowerctl*)lua_touserdata(L, 1);
  if (!ctx) {
    luaL_error(L, "failed to get keyboard upowerctl context from arg #1");
    return 0;
  }

  const int value = (int)luaL_checkinteger(L, 2);
  if (!upow_set(ctx, value)) {
    luaL_error(L, "failed to set keyboard brightness using upowerctl");
    return 0;
  }
  return 0;
}

static inline int get_max(lua_State* L) {
  UPowerctl* ctx = (UPowerctl*)lua_touserdata(L, 1);
  if (!ctx) {
    luaL_error(L, "failed to get keyboard upowerctl context from arg #1");
    return 0;
  }
  upow_brightness_t value = 0;
  if (!upow_get_max(ctx, &value)) {
    luaL_error(L, "failed to get keyboard brightness from upower");
    return 0;
  }
  lua_pushnumber(L, (lua_Number)value);
  return 1;
}

// clang-format off
static const luaL_Reg kKeyboardFuncs[] = {
  {"set", set_brightness},
  {"get", get_brightness},
  {"get_max", get_max},
};
// clang-format on

void init_keyboard_metatable(lua_State* L) {
  luaL_newmetatable(L, kKeyboardMetatableName);
  luaL_setfuncs(L, kKeyboardFuncs, 0);
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");
}
