#include "box.h"

#include <lauxlib.h>

#include "app.h"
#include "moonbar.h"
#include "state_lua.h"

Box* mbar_create_box(BarApp* app, const int orientation) {
  ASSERT(app);
  return (Box*)new_widget(app, gtk_box_new(orientation, 1), sizeof(Box));
}

void mbarL_push_box(BarApp* app, const int orientation) {
  Box* value = mbar_create_box(app, orientation);
#define L app->L
  if (!value) {
    luaL_error(L, "failed to create Box.");
    return;
  }
  mbarL_pushbox(L, value);
#undef L
}

void mbar_free_box(Box* value) {
  if (!value)
    return;
  // TODO(@s0cks): implement
  free(value);
}

DEFINE_LUA_TYPE_LIB(box, Box);

DEFINE_LUA_TYPE_INIT_F(box) {
  BarApp* app = mbarL_get_mbar_app(L);
  if (!app) {
    luaL_error(L, "failed to get global bar state");
    return 0;
  }
  mbarL_push_box(app, (int)lua_tonumber(L, 1));
  return 1;
}

DEFINE_LUA_F(box_append) {
  Box* box = mbarL_check_box(L, 1);
  ASSERT(box);
  GtkWidget* widget = mbarL_check_widget(L, 2);
  ASSERT(widget);
  gtk_box_append(GTK_BOX(box->handle), widget);
  return 1;
}

DEFINE_LUA_F(box_prepend) {
  Box* box = mbarL_check_box(L, 1);
  ASSERT(box);
  GtkWidget* widget = mbarL_check_widget(L, 2);
  gtk_box_append(GTK_BOX(box->handle), widget);
  return 1;
}

DECLARE_LUA_METATABLE(Box){
    {"append", box_append},
    {"prepend", box_prepend},
    {nullptr, nullptr},
};
DEFINE_LUA_INITWIDGETMETATABLE(box, Box);
