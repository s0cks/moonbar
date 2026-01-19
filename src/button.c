#include "button.h"

#include "app.h"
#include "moonbar.h"
#include "state_lua.h"
#include "widget.h"

FOR_EACH_BUTTON_SIGNAL(DEFINE_ON_WIDGET_SIGNAL);

static inline GtkWidget* create_gtk_button(const char* text) {
  GtkWidget* button = text != NULL ? gtk_button_new_with_label(text) : gtk_button_new();
  // TODO(@s0cks): do something?
  return button;
}

Button* mbar_create_button(BarApp* app, const char* text) {
  ASSERT(app);
  Button* widget = (Button*)malloc(sizeof(Button));
  if (!widget) {
    mbar_error(app, "failed to create Button");
    return nullptr;
  }

  GtkWidget* button = create_gtk_button(text);
  if (!button) {
    mbar_error(app, "failed to create GtkButton");
    return nullptr;
  }
  mbar_widget_init(widget, app, button, FOR_EACH_BUTTON_SIGNAL);
  return widget;
}

void mbarL_push_button(BarApp* app, const char* text) {
  ASSERT(app);
  Button* value = mbar_create_button(app, text);
#define L app->L
  if (!value) {
    luaL_error(L, "failed to create Button.");
    return;
  }
  mbarL_pushbutton(L, value);
#undef L
}

void mbar_free_button(Button* btn) {
  if (!btn)
    return;
  event_route_free(btn->owner->L, btn->events);
  g_object_unref(btn->handle);
  free(btn);
}

DEFINE_LUA_TYPE_LIB(button, Button);

DEFINE_LUA_TYPE_INIT_F(button) {
  const char* text = nullptr;
  if (lua_isnoneornil(L, -1)) {
    text = nullptr;
  } else {
    text = lua_tostring(L, -1);
  }

  BarApp* app = mbarL_get_mbar_app(L);
  if (!app) {
    luaL_error(L, "failed to get global bar state");
    return 0;
  }
  mbarL_push_button(app, text);
  return 1;
}

DEFINE_LUA_F(set_text) {
  Button* button = (Button*)lua_touserdata(L, 1);
  if (button == NULL) {
    luaL_error(L, "invalid button userdata");
    return 0;
  }
  const char* text = lua_tostring(L, -1);
  gtk_button_set_label(GTK_BUTTON(button->handle), text);
  return 1;
}

DEFINE_LUA_F(get_text) {
  Button* button = (Button*)lua_touserdata(L, 1);
  if (button == NULL) {
    luaL_error(L, "invalid button userdata");
    return 0;
  }
  const char* text = gtk_button_get_label(GTK_BUTTON(button->handle));
  lua_pushstring(L, text);
  return 1;
}

DECLARE_LUA_METATABLE(Button){
    {"get_text", get_text},
    {"set_text", set_text},
    {nullptr, nullptr},
};

DEFINE_LUA_INITWIDGETMETATABLE(button, Button);
