#include "moonbar.h"
#include "label.h"
#include "state_lua.h"

DEFINE_LUA_F(set_text) {
  Label* label = (Label*)lua_touserdata(L, 1);
  if(label == NULL) {
    luaL_error(L, "invalid label userdata");
    return 0;
  }
  const char* text = lua_tostring(L, 2);
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

DECLARE_LUA_METATABLE(Label) {
  { "get_text", get_text },
  { "set_text", set_text },
  {NULL, NULL},
};
DEFINE_LUA_INITWIDGETMETATABLE(label, Label);
