#ifndef BAR_BINDINGS_H
#define BAR_BINDINGS_H

#include "mybar.h"
#include "mybar/luabindings.h"

LUALIB_API int luaopen_bar_bindings(lua_State* L);

#endif // BAR_BINDINGS_H
