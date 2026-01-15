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

DEFINE_LUA_F(spawn_process) {
  const int num_args = lua_gettop(L) - 1;
  DLOG_F("number of args: %d\n", num_args);
  mbarL_check_global_app(L, app);
  int argc = 0;
  char** argv = NULL;
  if(lua_isstring(L, 1)) {
    argc = 1;
    argv = (char**)malloc(sizeof(char*) * (argc + 1));
    argv[0] = strdup(lua_tostring(L, 1));
    argv[1] = NULL;
  } else if(lua_istable(L, 1)) {
    argc = lua_objlen(L, 1);
    argv = (char**)malloc(sizeof(char*) * (argc + 1));
    for(int i = 0; i < argc; i++) {
      lua_rawgeti(L, 1, i + 1);
      luaL_checkstring(L, -1);
      argv[i] = strdup(lua_tostring(L, -1));
    }
    argv[argc + 1] = NULL;
  } else {
    luaL_error(L, "invalid value for argument 1");
    return 0;
  }

  ASSERT(argv != NULL);
  ASSERT(argc > 0);
  Process* proc = mbar_proc_new(app, argv, argc);
  if(!proc) {
    luaL_error(L, "failed to spawn new process");
    return 0;
  }

  // on_success
  if(num_args >= 2) {
    luaL_checktype(L, 2, LUA_TFUNCTION);
    mbar_proc_set_on_success(proc, L, 2);
  }

  // on_failure
  if(num_args >= 3) {
    luaL_checktype(L, 3, LUA_TFUNCTION);
    mbar_proc_set_on_failure(proc, L, 3);
  }
  
  // on_finished
  if(num_args >= 4) {
    luaL_checktype(L, 4, LUA_TFUNCTION);
    mbar_proc_set_on_finished(proc, L, 4);
  }

  mbar_proc_spawn(proc);
  return 0;
}

static const char* kLibName = "CoreLib";
static const luaL_Reg kLibFuncs[] = {
  { "is_debug", is_debug },
  { "get_config_dir", get_config_dir },
  { "get_cwd", get_cwd },
  { "on", on },
  { "next_tick", next_tick },
  { "append_left", append_left },
  { "append_center", append_center },
  { "append_right", append_right },
  { "spawn_process", spawn_process },
  { NULL, NULL },
};

static inline void
init_metatables(lua_State* L) {
  ASSERT(L);
  mbarL_initmetatable_widget(L);
  mbarL_initmetatable_label(L);
  mbarL_initmetatable_button(L);
  mbarL_initmetatable_event_route(L);
}

static inline void
init_libs(lua_State* L) {
  ASSERT(L);
  mbarL_initlib_label(L);
  mbarL_initlib_button(L);
  mbarL_initlib_event_route(L);
}

void mbarL_init_api(lua_State* L) {
  lua_newtable(L);
  luaL_setfuncs(L, kLibFuncs, 0);
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");
  lua_setglobal(L, kLibName);

  init_metatables(L);
  init_libs(L);
}
