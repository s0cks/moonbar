#include "moonbar.h"
#include "button.h"
#include "state_lua.h"
#include "moonbar/luabindings.h"

#define INVALID_BUTTON_USERDATA ({            \
  luaL_error(L, "invalid Button userdata.");  \
  return 0;                                   \
})

#define BUTTON_USERDATA(Name, Index)                            \
  Button* Name = (Button*)lua_touserdata(L, (Index));   \
  if((Name) == NULL)                                      \
    INVALID_BUTTON_USERDATA;

#define DECLARE_SIGNAL_CALLBACK(Name, Signal) \
  static inline void on_##Name##_callback(GtkWidget*, gpointer);
FOR_EACH_BUTTON_SIGNAL(DECLARE_SIGNAL_CALLBACK)
#undef DECLARE_SIGNAL_CALLBACK

#define DEFINE_LUA_BUTTON_SIGNAL_F(Name, Signal) \
  static inline void      \
  on_##Name##_callback(GtkWidget* widget, gpointer data) { \
    Button* value = (Button*)data; \
    if(value->callbacks[kButton##Name##Sig] == LUA_NOREF) { \
      return; \
    } \
    lua_rawgeti(value->owner->L, LUA_REGISTRYINDEX, value->callbacks[kButton##Name##Sig]); \
    if(lua_pcall(value->owner->L, 0, 0, 0) != LUA_OK) { \
      const char* err = lua_tostring(value->owner->L, -1); \
      fprintf(stderr, "lua error: %s\n", err); \
      exit(1); \
    } \
  } \
  DEFINE_LUA_F(on_##Name) {                 \
    BUTTON_USERDATA(value, 1);              \
    luaL_checktype(L, 2, LUA_TFUNCTION);    \
    lua_pushvalue(L, 2);                    \
    value->callbacks[kButton##Name##Sig] = luaL_ref(L, LUA_REGISTRYINDEX);                       \
    g_signal_connect(value->handle, #Signal, G_CALLBACK(on_##Name##_callback), value);               \
    return 0;       \
  }

FOR_EACH_BUTTON_SIGNAL(DEFINE_LUA_BUTTON_SIGNAL_F)

#define _ON_SIGNAL(Signal) #Signal
#define ON_SIGNAL(Signal) _ON_SIGNAL(on_##Signal)

static const luaL_Reg kButtonFuncs[] = {
#define BIND_BUTTON_SIGNAL_F(Name, Signal) \
  { ON_SIGNAL(Signal), on_##Name },

  FOR_EACH_BUTTON_SIGNAL(BIND_BUTTON_SIGNAL_F)
#undef BIND_BUTTON_SIGNAL_F
  { NULL, NULL },
};

DEFINE_LUA_F(button_new) {
  const char* text = NULL;
  if(lua_isnoneornil(L, -1)) {
    text = NULL;
  } else if(lua_isstring(L, -1)) {
    text = lua_tostring(L, -1);
  } else {
    text = lua_tostring(L, -1);
  }

  BarApp* app = mbarL_get_mbar_app(L);
  if(!app) {
    luaL_error(L, "failed to get global bar state");
    return 0;
  }

  Button* value = mbar_create_button(app, text);
  if(!value) {
    luaL_error(L, "failed to create gtk button.");
    return 0;
  }
  mbarL_pushbutton(L, value);
  return 1;
}

static const luaL_Reg kButtonLibFuncs[] = {
  { "new", button_new },
  { NULL, NULL },
};

void mbarL_initmetatable_button(lua_State* L) {
  luaL_newmetatable(L, "Button");
  luaL_setfuncs(L, kButtonFuncs, 0);
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");

  lua_newtable(L);
  luaL_setfuncs(L, kButtonLibFuncs, 0);
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");
  lua_setglobal(L, "ButtonLib");
}
