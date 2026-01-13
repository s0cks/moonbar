#include "widget_bindings.h"

#define BINDINGS_LIB_NAME widget
#define FOR_EACH_LIB_FUNC(V) \
  V(unref)

DECLARE_LUAOPEN_ENTRY_FUNCS(BINDINGS_LIB_NAME, FOR_EACH_LIB_FUNC);

static inline int
unref(lua_State* L) {
  GtkWidget* widget = (GtkWidget*)lua_touserdata(L, 1);
  if(widget == NULL) {
    luaL_error(L, "invalid Button userdata");
    return 0;
  }
  g_object_unref(widget);
  return 0;
}

DEFINE_LUAOPEN_ENTRY(BINDINGS_LIB_NAME);
