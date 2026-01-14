#ifndef MBAR_BINDINGS_H
#define MBAR_BINDINGS_H

#include "moonbar.h"
#include "moonbar/luabindings.h"

LUALIB_API int luaopen_mbar_bindings(lua_State* L);

#endif // MBAR_BINDINGS_H
