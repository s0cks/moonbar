#include "label_bindings.h"
#include "mybar.h"
#include "mybar/luabindings.h"

LUALIB_API int luaopen_label_bindings(lua_State* L) {
  luaL_newlib(L, kLabelLib);
  return 1;
}
