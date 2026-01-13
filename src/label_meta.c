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

void barL_initmetatable_label(lua_State* L) {
  luaL_newmetatable(L, kLabelMetatableName);
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");
  luaL_register(L, NULL, kLabelFuncs);
}
