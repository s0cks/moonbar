#include "moonbar.h"
#include "box.h"
#include "state_lua.h"

DEFINE_LUA_F(box_new) {
  BarApp* app = mbarL_get_mbar_app(L);
  if(!app) {
    luaL_error(L, "failed to get global bar state");
    return 0;
  }

  int orientation = GTK_ORIENTATION_HORIZONTAL;
  Box* value = mbar_create_box(app, orientation);
  if(!value) {
    luaL_error(L, "failed to create gtk box.");
    return 0;
  }
  mbarL_pushbox(L, value);
  return 1;
}

DECLARE_LUA_LIB(Box) {
  { "new", box_new },
  { NULL, NULL },
};

DEFINE_LUA_INITLIB(box);
