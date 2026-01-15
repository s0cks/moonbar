#include "moonbar.h"
#include "button.h"
#include "state_lua.h"

#define INVALID_BUTTON_USERDATA ({            \
  luaL_error(L, "invalid Button userdata.");  \
  return 0;                                   \
})

#define BUTTON_USERDATA(Name, Index)                    \
  Button* Name = (Button*)lua_touserdata(L, (Index));   \
  if((Name) == NULL)                                    \
    INVALID_BUTTON_USERDATA;

DEFINE_LUA_F(button_on) {
  BUTTON_USERDATA(button, 1);
  const char* event = lua_tostring(L, 2);
  if(!lua_isfunction(L, 3)) {
    luaL_error(L, "expected argument 3 to be a function\n");
    return 0;
  }
  DLOG("inserting event handler");
  event_route_insertl(button->events, event, L, 3);
  return 0;
}

static const luaL_Reg kButtonFuncs[] = {
  { "on", button_on },
  { NULL, NULL },
};

DEFINE_LUA_F(button_new) {
  const char* text = NULL;
  if(lua_isnoneornil(L, -1)) {
    text = NULL;
  } else if(lua_isstring(L, -1)) {
    text = lua_tostring(L, -1);
  } else {
    text = lua_tostring(L, -1);
  }

  BarApp* app = mbarL_get_mbar_app(L);
  if(!app) {
    luaL_error(L, "failed to get global bar state");
    return 0;
  }

  Button* value = mbar_create_button(app, text);
  if(!value) {
    luaL_error(L, "failed to create gtk button.");
    return 0;
  }
  mbarL_pushbutton(L, value);
  return 1;
}

static const luaL_Reg kButtonLibFuncs[] = {
  { "new", button_new },
  { NULL, NULL },
};

void mbarL_initmetatable_button(lua_State* L) {
  luaL_newmetatable(L, kButtonMetatableName);
  luaL_setfuncs(L, kButtonFuncs, 0);
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");

  lua_newtable(L);
  luaL_setfuncs(L, kButtonLibFuncs, 0);
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");
  lua_setglobal(L, "ButtonLib");
}
