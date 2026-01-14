#include "mybar.h"
#include "state_lua.h"
#include "mybar/luabindings.h"
#include "util.h"

DEFINE_LUA_F(is_debug) {
  lua_pushboolean(L, BAR_DEBUG);
  return 1;
}

DEFINE_LUA_F(get_config_dir) {
  barL_check_global_app(L, app);
  lua_pushstring(L, app->home);
  return 1;
}

DEFINE_LUA_F(get_cwd) {
  barL_check_global_app(L, app);
  lua_pushstring(L, bar_get_cwd());
  return 1;
}

DEFINE_LUA_F(on) {
  barL_check_global_app(L, app);
  const char* event = lua_tostring(L, 1);
  event_route_insertl(app->events, event, L, 2);
  return 0;
}

static inline GtkWidget**
widget_ptr(lua_State* L, const int index) {
  return (GtkWidget**)lua_touserdata(L, index);
}

DEFINE_LUA_F(add_left) {
  barL_check_global_app(L, app);
  GtkWidget* widget = *(widget_ptr(L, 1));
  gtk_box_append(GTK_BOX(app->left), widget);
  return 0;
}

DEFINE_LUA_F(add_center) {
  barL_check_global_app(L, app);
  GtkWidget* widget = *(widget_ptr(L, 1));
  gtk_box_append(GTK_BOX(app->center), widget);
  return 0;
}

DEFINE_LUA_F(add_right) {
  barL_check_global_app(L, app);
  GtkWidget* widget = *(widget_ptr(L, 1));
  gtk_box_append(GTK_BOX(app->right), widget);
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
  { NULL, NULL },
};

void barL_init_api(lua_State* L) {
  lua_newtable(L);
  luaL_setfuncs(L, kLibFuncs, 0);
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");
  lua_setglobal(L, "CoreLib");

  barL_initmetatable_label(L);
  barL_initmetatable_button(L);
}
