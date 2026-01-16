#include "moonbar.h"
#include "box.h"
#include "state_lua.h"

DECLARE_LUA_METATABLE(Box) {
  { NULL, NULL },
};
DEFINE_LUA_INITWIDGETMETATABLE(box, Box);
