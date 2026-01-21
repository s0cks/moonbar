#ifndef MBAR_BUTTON_H
#define MBAR_BUTTON_H

#include "moonbar.h"
#include "widget.h"

// clang-format off
#define FOR_EACH_BUTTON_SIGNAL(V) \
  FOR_EACH_WIDGET_SIGNAL(V)       \
  V(clicked)
// clang-format on

struct _Button {
  MOONBAR_WIDGET_FIELDS;
};

static inline Button* mbarL_check_button(lua_State* L, const int index) {
  return (Button*)luaL_checkudata(L, index, kButtonMetatableName);
}

#endif  // MBAR_BUTTON_H
