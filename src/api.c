#include "moonbar.h"
#include "state_lua.h"
#include "moonbar/luabindings.h"
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

DEFINE_LUA_F(add_left) {
  mbarL_check_global_app(L, app);
  GtkWidget* widget = *(widget_ptr(L, 1));
  gtk_box_append(GTK_BOX(app->left), widget);
  return 0;
}

DEFINE_LUA_F(add_center) {
  mbarL_check_global_app(L, app);
  GtkWidget* widget = *(widget_ptr(L, 1));
  gtk_box_append(GTK_BOX(app->center), widget);
  return 0;
}

DEFINE_LUA_F(add_right) {
  mbarL_check_global_app(L, app);
  GtkWidget* widget = *(widget_ptr(L, 1));
  gtk_box_append(GTK_BOX(app->right), widget);
  return 0;
}

DEFINE_LUA_F(on_next_tick) {
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
  { "add_left", add_left },
  { "add_center", add_center },
  { "add_right", add_right },
  { "on_next_tick", on_next_tick },
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
