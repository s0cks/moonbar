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

#define DECLARE_INITMETATABLE(Name)                 \
  void mbarL_initmetatable_##Name(lua_State* L);
DECLARE_INITMETATABLE(widget);
DECLARE_INITMETATABLE(label);
DECLARE_INITMETATABLE(button);
DECLARE_INITMETATABLE(event_route);
DECLARE_INITMETATABLE(box);
#undef DECLARE_INITMETATABLE

#define DECLARE_INITLIB(Name)                 \
  void mbarL_initlib_##Name(lua_State* L);
DECLARE_INITLIB(label);
DECLARE_INITLIB(button);
DECLARE_INITLIB(event_route);
DECLARE_INITLIB(box);
#undef DECLARE_INITLIB

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

#define DECLARE_LUA_METATABLE(Name)           \
  static const luaL_Reg k##Name##Funcs[] =

#define DECLARE_LUA_LIB(Name)                   \
  static const char* kLibName = #Name "Lib";    \
  static const luaL_Reg kLibFuncs[] =

#define DEFINE_LUA_INITMETATABLE(Name, Type)        \
  void mbarL_initmetatable_##Name(lua_State* L) {   \
    luaL_newmetatable(L, k##Type##MetatableName);   \
    luaL_setfuncs(L, k##Type##Funcs, 0);            \
    lua_pushvalue(L, -1);                           \
    lua_setfield(L, -2, "__index");                 \
  }
#define DEFINE_LUA_INITWIDGETMETATABLE(Name, Type)  \
  void mbarL_initmetatable_##Name(lua_State* L) {   \
    luaL_newmetatable(L, k##Type##MetatableName);   \
    luaL_setfuncs(L, k##Type##Funcs, 0);            \
    luaL_getmetatable(L, kWidgetMetatableName);      \
    lua_setfield(L, -2, "__index");                 \
  }

#define DEFINE_LUA_INITLIB(Name)                \
  void mbarL_initlib_##Name(lua_State* L) {     \
    lua_newtable(L);                            \
    luaL_setfuncs(L, kLibFuncs, 0);             \
    lua_pushvalue(L, -1);                       \
    lua_setfield(L, -2, "__index");             \
    lua_setglobal(L, kLibName);                 \
  }

#define _INVALID_WIDGET_USERDATA(L, Type) ({    \
  luaL_error(L, "invalid %s userdata", #Type);  \
  return 0;                                     \
})
#define _GET_WIDGET_USERDATA(L, Name, Type, Index)  \
  Type* Name = (Type*)lua_touserdata(L, (Index));   \
  if((Name) == NULL) _INVALID_WIDGET_USERDATA(L, Type);

#define mbarL_error_invalid_button_userdata(L) \
  _INVALID_WIDGET_USERDATA(L, Button)
#define mbarL_get_button_userdata(L, Name, Index) \
  _GET_WIDGET_USERDATA(L, Name, Button, Index)

#define mbarL_error_invalid_label_userdata(L) \
  _INVALID_WIDGET_USERDATA(L, Label)
#define mbarL_get_label_userdata(L, Name, Index) \
  _GET_WIDGET_USERDATA(L, Name, Label, Index)

#define mbarL_error_invalid_event_route_userdata(L) \
  _INVALID_WIDGET_USERDATA(L, EventRoute)
#define mbarL_get_event_route_userdata(L, Name, Index) \
  _GET_WIDGET_USERDATA(L, Name, EventRoute, Index)

#endif // MBAR_LSTATE_H
