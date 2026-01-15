#include "moonbar.h"
#include "state_lua.h"
#include "util.h"

DEFINE_LUA_F(is_debug) {
  lua_pushboolean(L, MBAR_DEBUG);
  return 1;
}

DEFINE_LUA_F(get_config_dir) {
  mbarL_check_global_app(L, app);
  lua_pushstring(L, app->home);
  return 1;
}

DEFINE_LUA_F(get_cwd) {
  mbarL_check_global_app(L, app);
  lua_pushstring(L, mbar_get_cwd());
  return 1;
}

DEFINE_LUA_F(on) {
  mbarL_check_global_app(L, app);
  const char* event = lua_tostring(L, 1);
  event_route_insertl(app->events, event, L, 2);
  return 0;
}

static inline GtkWidget**
widget_ptr(lua_State* L, const int index) {
  return (GtkWidget**)lua_touserdata(L, index);
}

static inline GtkWidget*
get_widget_ptr_param(lua_State* L, int index) {
  if(lua_isfunction(L, index)) {
    int status = lua_pcall(L, 1, 0, 0);
    if(status != LUA_OK) {
      luaL_error(L, "failed to execute callback");
      return 0;
    }
    index = -1;
  }
  return *(widget_ptr(L, index));
}

#define DEFINE_LUA_APPEND_F(Side)                           \
  DEFINE_LUA_F(append_##Side) {                             \
    mbarL_check_global_app(L, app);                         \
    mbar_append_##Side(app, get_widget_ptr_param(L, 1));    \
    return 0;                                               \
  }
DEFINE_LUA_APPEND_F(left);
DEFINE_LUA_APPEND_F(center);
DEFINE_LUA_APPEND_F(right);
#undef DEFINE_LUA_APPEND_F

DEFINE_LUA_F(next_tick) {
  mbarL_check_global_app(L, app);
  Callback* cb = (Callback*)malloc(sizeof(Callback));
  if(!cb) {
    luaL_error(L, "failed to create Callback");
    return 0;
  }
  cb_initbg_lfunc(cb, L, 1);
  cb_list_append(&app->next_tick_listeners, cb);
  return 0;
}

static const luaL_Reg kLibFuncs[] = {
  { "is_debug", is_debug },
  { "get_config_dir", get_config_dir },
  { "get_cwd", get_cwd },
  { "on", on },
  { "next_tick", next_tick },
  { "append_left", append_left },
  { "append_center", append_center },
  { "append_right", append_right },
  { NULL, NULL },
};

void mbarL_init_api(lua_State* L) {
  lua_newtable(L);
  luaL_setfuncs(L, kLibFuncs, 0);
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");
  lua_setglobal(L, "CoreLib");

  mbarL_initmetatable_label(L);
  mbarL_initmetatable_button(L);
}
