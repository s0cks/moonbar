#include "box.h"

#include "app.h"
#include "moonbar.h"
#include "state_lua.h"

Box* mbar_create_box(BarApp* app, const int orientation) {
  ASSERT(app);
  fprintf(stdout, "creating box\n");
  GtkWidget* widget = gtk_box_new(orientation, 1);
  if (!widget) {
    mbar_error(app, "failed to create Box");
    return nullptr;
  }
  Box* box = (Box*)malloc(sizeof(Box));
  box->owner = app;
  box->events = nullptr;
  box->handle = widget;
  g_object_ref(box->handle);
  return box;
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
  const int orientation = (int)lua_tonumber(L, 1);
  mbarL_push_box(app, orientation);
  return 1;
}

DEFINE_LUA_F(box_append) {
  fprintf(stdout, "appending\n");
  Box* box = (Box*)lua_touserdata(L, 1);
  if (box == NULL) {
    luaL_error(L, "invalid Box userdata");
    return 0;
  }
  fprintf(stdout, "adding userdata at 2 to box\n");
  const void* data = lua_touserdata(L, 2);
  if (!data) {
    luaL_error(L, "invalid widget for param 1");
    return 0;
  }
  fprintf(stdout, "appending widget to gtk box\n");
  GtkWidget* widget = mbar_widget_get_handle(data);
  gtk_box_append(GTK_BOX(box->handle), widget);
  return 1;
}

DEFINE_LUA_F(box_prepend) {
  Box* box = (Box*)lua_touserdata(L, 1);
  if (box == NULL) {
    luaL_error(L, "invalid Box userdata");
    return 0;
  }
  const void* data = lua_touserdata(L, 2);
  if (!data) {
    luaL_error(L, "invalid widget for param 1");
    return 0;
  }
  GtkWidget* widget = mbar_widget_get_handle(data);
  gtk_box_append(GTK_BOX(box->handle), widget);
  return 1;
}

DECLARE_LUA_METATABLE(Box){
    {"append", box_append},
    {"prepend", box_prepend},
    {nullptr, nullptr},
};
DEFINE_LUA_INITWIDGETMETATABLE(box, Box);
