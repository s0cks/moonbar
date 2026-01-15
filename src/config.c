#include "moonbar.h"
#include "app.h"
#include "state_lua.h"

const char* mbar_config_gets_or(BarApp* app, const char* name, const char* default_value) {
  ASSERT(app);
  ASSERT(name);
#define L app->L
  mbarL_push_config_ref(app);
  lua_getfield(L, -1, name);
  if(lua_isnoneornil(L, -1))
    return NULL;
  return lua_tostring(L, -1);
#undef L
}

bool mbar_config_getb_or(BarApp* app, const char* name, const bool default_value) {
  ASSERT(app);
  ASSERT(name);
#define L app->L
  mbarL_push_config_ref(app);
  lua_getfield(L, -1, name);
  if(!lua_isboolean(L, -1))
    return default_value;
  return lua_toboolean(L, -1);
#undef L
}

int mbar_config_geti_or(BarApp* app, const char* name, const int default_value) {
#define L app->L
  mbarL_push_config_ref(app);
  lua_getfield(L, -1, name);
  if(!lua_isnumber(L, -1))
    return default_value;
  return lua_tonumber(L, -1);
#undef L
}
