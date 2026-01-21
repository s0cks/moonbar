#ifndef MBAR_BOX_H
#define MBAR_BOX_H

#include "moonbar.h"
#include "widget.h"

// clang-format off
#define FOR_EACH_BOX_SIGNAL(V) \
  FOR_EACH_WIDGET_SIGNAL(V)
// clang-format on

struct _Box {
  MOONBAR_WIDGET_FIELDS;
};

static inline Box* mbarL_check_box(lua_State* L, const int index) {
  ASSERT(L);
  return (Box*)luaL_checkudata(L, index, kBoxMetatableName);
}

#endif  // MBAR_BOX_H
