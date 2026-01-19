#include "api.h"

#include "file_line_reader.h"
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

static inline GtkWidget** widget_ptr(lua_State* L, const int index) {
  return (GtkWidget**)lua_touserdata(L, index);
}

static inline GtkWidget* get_widget_ptr_param(lua_State* L, int index) {
  if (lua_isfunction(L, index)) {
    int status = lua_pcall(L, 1, 0, 0);
    if (status != LUA_OK) {
      luaL_error(L, "failed to execute callback");
      return nullptr;
    }
    index = -1;
  }
  return *(widget_ptr(L, index));
}

#define DEFINE_LUA_APPEND_F(Side)                        \
  DEFINE_LUA_F(append_##Side) {                          \
    mbarL_check_global_app(L, app);                      \
    mbar_append_##Side(app, get_widget_ptr_param(L, 1)); \
    return 0;                                            \
  }
DEFINE_LUA_APPEND_F(left);
DEFINE_LUA_APPEND_F(center);
DEFINE_LUA_APPEND_F(right);
#undef DEFINE_LUA_APPEND_F

DEFINE_LUA_F(next) {
  mbarL_check_global_app(L, app);
  Callback* cb = (Callback*)malloc(sizeof(Callback));
  if (!cb) {
    luaL_error(L, "failed to create Callback");
    return 0;
  }
  cb_initbg_lfunc(cb, L, 1);
  cb_list_append(&app->next_tick_listeners, cb);
  return 0;
}

static const char* kLibName = "CoreLib";
static const luaL_Reg kLibFuncs[] = {
    {"on", on},
    {"is_debug", is_debug},
    {"get_config_dir", get_config_dir},
    {"get_cwd", get_cwd},
    {"next", next},

    {"append_left", append_left},
    {"append_center", append_center},
    {"append_right", append_right},

    {"spawn", mbarL_api_spawn},
    {"read_lines", mbarL_api_read_lines},
    {"watch", mbarL_api_watch},
    {nullptr, nullptr},
};

void mbarL_init_api(lua_State* L) {
  ASSERT(L);
  LUA_NEW_LIB_TABLE(kLibFuncs);
  lua_setglobal(L, kLibName);

#define DEFINE_CALL(Name, Type) mbarL_initmetatable_##Name(L);

  DEFINE_CALL(widget, Widget);
  FOR_EACH_LUA_TYPE(DEFINE_CALL);
#undef DEFINE_CALL

#define DEFINE_CALL(Name, Type) mbarL_initlib_##Name(L);

  FOR_EACH_LUA_TYPE(DEFINE_CALL);
#undef DEFINE_CALL
}
