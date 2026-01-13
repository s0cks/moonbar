#ifndef LABEL_BINDINGS_H
#define LABEL_BINDINGS_H

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <luajit.h>

LUALIB_API int luaopen_label(lua_State* L);

#endif // LABEL_BINDINGS_H
