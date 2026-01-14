#include "mybar.h"
#include "label.h"
#include "state_lua.h"
#include "mybar/luabindings.h"

DEFINE_LUA_F(set_text) {
  Label* label = (Label*)lua_touserdata(L, 1);
  if(label == NULL) {
    luaL_error(L, "invalid label userdata");
    return 0;
  }
  const char* text = lua_tostring(L, -1);
  gtk_label_set_text(GTK_LABEL(label->handle), text);
  return 1;
}

DEFINE_LUA_F(get_text) {
  Label* label = (Label*)lua_touserdata(L, 1);
  if(label == NULL) {
    luaL_error(L, "invalid label userdata");
    return 0;
  }
  const char* text = gtk_label_get_text(GTK_LABEL(label->handle));
  lua_pushstring(L, text);
  return 1;
}

static const luaL_Reg kLabelFuncs[] = {
  { "get_text", get_text },
  { "set_text", set_text },
  {NULL, NULL},
};

DEFINE_LUA_F(new_label) {
  const char* text = NULL;
  if(lua_isnoneornil(L, -1)) {
    text = "";
  } else if(lua_isstring(L, -1)) {
    text = lua_tostring(L, -1);
  } else {
    text = lua_tostring(L, -1);
  }

  BarApp* app = barL_get_bar_app(L);
  if(!app) {
    luaL_error(L, "failed to get global bar state");
    return 0;
  }

  Label* label = bar_create_label(app, text);
  if(!label) {
    luaL_error(L, "failed to create gtk label");
    return 0;
  }
  barL_pushlabel(L, label);
  return 1;
}

static const luaL_Reg kLabelLibFuncs[] = {
  {"new", new_label},
  {NULL, NULL}
};

void barL_initmetatable_label(lua_State* L) {
  luaL_newmetatable(L, kLabelMetatableName);
  luaL_setfuncs(L, kLabelFuncs, 0);
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");

  lua_newtable(L);
  luaL_setfuncs(L, kLabelLibFuncs, 0);
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");
  lua_setglobal(L, "LabelLib");
}
