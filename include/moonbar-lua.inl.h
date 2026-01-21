#define DECLARE_LUA_F(Name) MBAR_INLINE int Name(lua_State* L);

#define DEFINE_LUA_F(Name)  MBAR_INLINE int Name(lua_State* L)

BarApp* mbarL_get_mbar_app(lua_State* L);
void mbarL_dostring(BarApp* app, const char* code);
void mbarL_dofile(BarApp* app, const char* filename);

#define FOR_EACH_LUA_TYPE(V) \
  V(box, Box)                \
  V(label, Label)            \
  V(button, Button)          \
  V(event_route, EventRoute)

#define DECLARE_LUA_TYPE(Name, Type)                 \
  static const char* k##Type##MetatableName = #Type; \
  void mbarL_initmetatable_##Name(lua_State* L);     \
  void mbarL_initlib_##Name(lua_State* L);

#define DECLARE_LUA_TYPE_STACK_OPS(Name, Type)                        \
  void mbarL_push##Name(lua_State* L, Type* value);                   \
  MBAR_INLINE void mbarL_push##Name##x(BarApp* app, Type* value) {    \
    return mbarL_push##Name(app->L, value);                           \
  }                                                                   \
  Type* mbarL_to##Name(lua_State* L, const int index);                \
  MBAR_INLINE Type* mbarL_to##Name##x(BarApp* app, const int index) { \
    return mbarL_to##Name(app->L, index);                             \
  }

DECLARE_LUA_TYPE(widget, Widget)
FOR_EACH_LUA_TYPE(DECLARE_LUA_TYPE)
FOR_EACH_LUA_TYPE(DECLARE_LUA_TYPE_STACK_OPS)
#undef DECLARE_LUA_TYPE
#undef DECLARE_LUA_TYPE_STACK_OPS

#define LUA_NEW_LIB_TABLE(LibFuncs) \
  lua_newtable(L);                  \
  luaL_setfuncs(L, LibFuncs, 0);    \
  lua_pushvalue(L, -1);             \
  lua_setfield(L, -2, "__index");

#define DEFINE_LUA_TYPE_LIB(Name, Type) \
  DECLARE_LUA_F(new_##Name);            \
  DECLARE_LUA_LIB(Type){                \
      {"new", new_##Name},              \
      {NULL, NULL},                     \
  };                                    \
  DEFINE_LUA_INITLIB(Name);

#define DEFINE_LUA_TYPE_INIT_F(Name) DEFINE_LUA_F(new_##Name)

#define DECLARE_LUA_METATABLE(Name)  static const luaL_Reg k##Name##Funcs[] =

#define DECLARE_LUA_LIB(Name)                \
  static const char* kLibName = #Name "Lib"; \
  static const luaL_Reg kLibFuncs[] =

#define DEFINE_LUA_INITMETATABLE(Name, Type)      \
  void mbarL_initmetatable_##Name(lua_State* L) { \
    luaL_newmetatable(L, k##Type##MetatableName); \
    luaL_setfuncs(L, k##Type##Funcs, 0);          \
    lua_pushvalue(L, -1);                         \
    lua_setfield(L, -2, "__index");               \
  }
#define DEFINE_LUA_INITWIDGETMETATABLE(Name, Type) \
  void mbarL_initmetatable_##Name(lua_State* L) {  \
    luaL_newmetatable(L, k##Type##MetatableName);  \
    luaL_setfuncs(L, k##Type##Funcs, 0);           \
    lua_pushvalue(L, -1);                          \
    lua_setfield(L, -2, "__index");                \
    luaL_getmetatable(L, kWidgetMetatableName);    \
    lua_setmetatable(L, -2);                       \
    lua_pop(L, 1);                                 \
  }

#define DEFINE_LUA_INITLIB(Name)            \
  void mbarL_initlib_##Name(lua_State* L) { \
    lua_newtable(L);                        \
    luaL_setfuncs(L, kLibFuncs, 0);         \
    lua_pushvalue(L, -1);                   \
    lua_setfield(L, -2, "__index");         \
    lua_setglobal(L, kLibName);             \
  }

#define _INVALID_WIDGET_USERDATA(L, Type)        \
  ({                                             \
    luaL_error(L, "invalid %s userdata", #Type); \
    return 0;                                    \
  })
#define _GET_WIDGET_USERDATA(L, Name, Type, Index) \
  Type* Name = (Type*)lua_touserdata(L, (Index));  \
  if ((Name) == NULL)                              \
    _INVALID_WIDGET_USERDATA(L, Type);

#define mbarL_error_invalid_button_userdata(L)         _INVALID_WIDGET_USERDATA(L, Button)
#define mbarL_get_button_userdata(L, Name, Index)      _GET_WIDGET_USERDATA(L, Name, Button, Index)

#define mbarL_error_invalid_label_userdata(L)          _INVALID_WIDGET_USERDATA(L, Label)
#define mbarL_get_label_userdata(L, Name, Index)       _GET_WIDGET_USERDATA(L, Name, Label, Index)

#define mbarL_error_invalid_event_route_userdata(L)    _INVALID_WIDGET_USERDATA(L, EventRoute)
#define mbarL_get_event_route_userdata(L, Name, Index) _GET_WIDGET_USERDATA(L, Name, EventRoute, Index)

#define _DEFINE_LUAOPEN_MOONBAR_LIB_F(Name, LibFuncs) \
  extern int luaopen_moonbar_##Name(lua_State* L) {   \
    LUA_NEW_LIB_TABLE(LibFuncs);                      \
    return 1;                                         \
  }

// clang-format off
#define DEFINE_LUAOPEN_MOONBAR_LIB_F(Name) \
  _DEFINE_LUAOPEN_MOONBAR_LIB_F(Name, kLibFuncs)
// clang-format on
