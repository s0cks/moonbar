#ifndef MBAR_LABEL_H
#define MBAR_LABEL_H

#include "moonbar.h"
#include "widget.h"

// clang-format off
#define FOR_EACH_LABEL_SIGNAL(V) \
  FOR_EACH_WIDGET_SIGNAL(V)
// clang-format on

struct _Label {
  MOONBAR_WIDGET_FIELDS;
};

static inline Label* mbarL_check_label(lua_State* L, const int index) {
  return (Label*)luaL_checkudata(L, index, kLabelMetatableName);
}

#endif  // MBAR_LABEL_H
