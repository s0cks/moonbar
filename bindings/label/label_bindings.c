#include "label_bindings.h"
#include "moonbar.h"
#include "moonbar/luabindings.h"

LUALIB_API int luaopen_label_bindings(lua_State* L) {
  luaL_newlib(L, kLabelLib);
  return 1;
}
