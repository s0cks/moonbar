#include "button_bindings.h"
#include "mybar.h"

DEFINE_LUA_F(button_new) {
  const char* text = lua_tostring(L, -1);

  BarState* state = barL_get_bar_state(L);
  if(!state) {
    luaL_error(L, "failed to get global bar state");
    return 0;
  }

  fprintf(stdout, "creating label w/ text: %s\n", text);
  GtkWidget *label = gtk_label_new(text);
  if(!label) {
    luaL_error(L, "failed to create gtk label");
    return 0;
  }
  gtk_window_set_child(GTK_WINDOW(state->window), label);

  lua_pushlightuserdata(L, label);
  luaL_getmetatable(L, "Label");
  lua_setmetatable(L, -2);
  return 1;
}

static const luaL_Reg kButtonFuncs[] = {
  {NULL, NULL},
};

static const luaL_Reg kButtonLib[] = {
  {"new", button_new},
  {NULL, NULL}
};

LUALIB_API int luaopen_label(lua_State* L) {
  luaL_newmetatable(L, "Button");

  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");
  luaL_register(L, NULL, kButtonFuncs);

  luaL_newlib(L, kButtonLib);
  return 1;
}
