#include "backlight.h"

#include <lua.h>

#include "keyboard.h"

DEFINE_LUA_F(keyboard) {
  UPowerctl* ctx = upow_new();
  if (!ctx) {
    luaL_error(L, "failed to create upowerctl context");
    return 0;
  }

  lua_pushlightuserdata(L, ctx);
  luaL_getmetatable(L, kKeyboardMetatableName);
  lua_setmetatable(L, -2);
  return 1;
}

DEFINE_LUA_F(screen) {
  return 0;
}

// clang-format off
static const luaL_Reg kLibFuncs[] = {
  {"keyboard", keyboard},
  {"screen", screen},
  {nullptr, nullptr},
};
// clang-format on

extern int luaopen_moonbar_backlight(lua_State* L) {
  init_keyboard_metatable(L);

  LUA_NEW_LIB_TABLE(kLibFuncs);
  return 1;
}
