#include "moonbar.h"
#include "log.h"

void cb_init_cfunc(Callback* cb, const CallbackMode mode, void (*func_ptr)(void)) {
  ASSERT(cb);
  ASSERT(func_ptr);
  cb->next = cb->prev = NULL;
  cb->mode = mode;
  cb->type = kCFunc;
  cb->data = (uintptr_t)func_ptr;
}

void cb_init_lfunc(Callback* cb, const CallbackMode mode, lua_State* L, const int index) {
  ASSERT(cb);
  ASSERT(L);
  cb->next = cb->prev = NULL;
  cb->mode = mode;
  cb->type = kLFunc;
  lua_pushvalue(L, index);
  cb->data = (uintptr_t)luaL_ref(L, LUA_REGISTRYINDEX);
}

void cbL_pcall(lua_State* L, Callback* cb) {
  cbL_push(L, cb);
  int status = lua_pcall(L, 0, 0, 0);
  if(status != LUA_OK)
    luaL_error(L, "failed to execute callback");
}

void cb_ccall(Callback* cb) {
  return cb_get_func_ptr(cb)();
}

void cb_free(Callback* cb) {
  if(!cb)
    return;
  //TODO(@s0cks): free lua callback
  // if(cb_is_lua(cb))
  //   luaL_unref(L, LUA_REGISTRYINDEX, (int)cb->data);
  free(cb);
}
