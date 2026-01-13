#include "label_bindings.h"
#include "mybar.h"
#include "mybar/luabindings.h"

DEFINE_LUA_F(new_label) {
  const char* text = NULL;
  if(lua_isnoneornil(L, -1)) {
    text = "";
  } else if(lua_isstring(L, -1)) {
    text = lua_tostring(L, -1);
  } else {
    text = lua_tostring(L, -1);
  }

  BarState* state = barL_get_bar_state(L);
  if(!state) {
    luaL_error(L, "failed to get global bar state");
    return 0;
  }

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

DEFINE_LUA_F(label_set_text) {
  GtkWidget* label = (GtkWidget*)lua_touserdata(L, 1);
  if(label == NULL) {
    luaL_error(L, "invalid label userdata");
    return 0;
  }
  const char* text = lua_tostring(L, -1);
  gtk_label_set_text(GTK_LABEL(label), text);
  return 1;
}

DEFINE_LUA_F(label_get_text) {
  GtkWidget* label = (GtkWidget*)lua_touserdata(L, 1);
  if(label == NULL) {
    luaL_error(L, "invalid label userdata");
    return 0;
  }
  const char* text = gtk_label_get_text(GTK_LABEL(label));
  lua_pushstring(L, text);
  return 1;
}

DEFINE_LUA_F(label_unref) {
  GtkWidget* label = (GtkWidget*)lua_touserdata(L, 1);
  if(label == NULL) {
    luaL_error(L, "invalid label userdata");
    return 0;
  }
  g_object_unref(label);
  return 0;
}

static const luaL_Reg kLabelFuncs[] = {
  {"get_text", label_get_text},
  {"set_text", label_set_text},
  {"unref", label_unref},
  {NULL, NULL},
};

static const luaL_Reg kLabelLib[] = {
  {"new", new_label},
  {NULL, NULL}
};

LUALIB_API int luaopen_label_bindings(lua_State* L) {
  luaL_newmetatable(L, "Label");

  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");
  luaL_register(L, NULL, kLabelFuncs);

  luaL_newlib(L, kLabelLib);
  return 1;
}
