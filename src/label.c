#include "label.h"

#include "app.h"
#include "label.h"
#include "moonbar.h"
#include "state_lua.h"

Label* mbar_create_label(BarApp* app, const char* text) {
  GtkWidget* widget = gtk_label_new(text);
  if (!widget) {
    mbar_error(app, "failed to create Label");
    return NULL;
  }
  gtk_label_set_single_line_mode(GTK_LABEL(widget), TRUE);
  gtk_label_set_xalign(GTK_LABEL(widget), 0.5);
  gtk_label_set_yalign(GTK_LABEL(widget), 0.5);
  gtk_label_set_justify(GTK_LABEL(widget), GTK_JUSTIFY_CENTER);
  return new_widget(app, widget, sizeof(Label));
}

void mbarL_push_new_label(BarApp* app, const char* text) {
  ASSERT(app);
  Label* label = mbar_create_label(app, text);
#define L app->L
  if (!label) {
    luaL_error(L, "failed to create Label");
    return;
  }
  mbarL_pushlabel(L, label);
#undef L
}

void mbar_free_label(Label* value) {
  if (!value)
    return;
  // TODO(@s0cks): should probably free callbacks in Label
  g_object_unref(value->handle);
  free(value);
}

DEFINE_LUA_TYPE_LIB(label, Label);

DEFINE_LUA_TYPE_INIT_F(label) {
  const char* text = lua_tostring(L, 1);
  BarApp* app = mbarL_get_mbar_app(L);
  if (!app) {
    luaL_error(L, "failed to get global bar state");
    return 0;
  }
  mbarL_push_new_label(app, text);
  return 1;
}

DEFINE_LUA_F(set_text) {
  Label* label = mbarL_check_label(L, 1);
  ASSERT(label);
  const char* text = lua_tostring(L, 2);
  gtk_label_set_text(GTK_LABEL(label->handle), text);
  return 1;
}

DEFINE_LUA_F(get_text) {
  Label* label = mbarL_check_label(L, 1);
  ASSERT(label);
  const char* text = gtk_label_get_text(GTK_LABEL(label->handle));
  lua_pushstring(L, text);
  return 1;
}

DECLARE_LUA_METATABLE(Label){
    {"get_text", get_text},
    {"set_text", set_text},
    {nullptr, nullptr},
};
DEFINE_LUA_INITWIDGETMETATABLE(label, Label);
