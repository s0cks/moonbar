#ifndef EVENTBINDINGS_H
#define EVENTBINDINGS_H

#include "mybar.h"
#include "mybar/luabindings.h"

LUALIB_API int luaopen_event_bindings(lua_State* L);

#endif // EVENTBINDINGS_H
