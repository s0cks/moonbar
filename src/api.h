#ifndef MOONBAR_API_H
#define MOONBAR_API_H

#include "moonbar.h"

// clang-format off
#define DECLARE_LUA_F0(Name) \
  int mbarL_api_##Name(lua_State* L);
// clang-format on

DECLARE_LUA_F0(next);
DECLARE_LUA_F0(watch);
DECLARE_LUA_F0(spawn);
DECLARE_LUA_F0(read_lines);

#endif  // MOONBAR_API_H
