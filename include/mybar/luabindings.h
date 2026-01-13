#ifndef BAR_LUABINDINGS_H
#define BAR_LUABINDINGS_H

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <luajit.h>

#define DEFINE_LUA_F(Name) \
  static inline int Name(lua_State* L)

#define _DECLARE_LUAOPEN_ENTRY(LibName)     \
  static const luaL_Reg LibName[] = 
#define DECLARE_LUAOPEN_ENTRY(Name) \
  _DECLARE_LUAOPEN_ENTRY(kLib_##Name)

#define __DECLARE_LUAOPEN_ENTRY_FUNC(Func) \
  DEFINE_LUA_F(Func);
#define __BIND_LUAOPEN_FUNC(Func) \
  { #Func, Func },
#define DECLARE_LUAOPEN_ENTRY_FUNCS(Name, ForEachFunc)    \
  ForEachFunc(__DECLARE_LUAOPEN_ENTRY_FUNC)               \
  DECLARE_LUAOPEN_ENTRY(Name) {                           \
    ForEachFunc(__BIND_LUAOPEN_FUNC)                      \
    {NULL, NULL}                                          \
  };

#define _DEFINE_LUAOPEN_ENTRY(Name, LibName)        \
  LUALIB_API int luaopen_##Name##_bindings(lua_State* L) {     \
    luaL_newlib(L, (LibName));                      \
    return 1;                                       \
  }
#define DEFINE_LUAOPEN_ENTRY(Name) \
  _DEFINE_LUAOPEN_ENTRY(Name, kLib_##Name)

#endif // BAR_LUABINDINGS_H
