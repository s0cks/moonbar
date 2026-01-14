#ifndef MBAR_LUA_CALLBACK_H
#define MBAR_LUA_CALLBACK_H

#include <stdint.h>
#include "moonbar.h"

typedef void (*CallbackFuncPtr)(void);
typedef struct _Callback Callback;

typedef enum {
  kForegroundCallback,
  kBackgroundCallback,
} CallbackMode;

typedef enum {
  kCFunc,
  kLFunc,
} CallbackType;

struct _Callback {
  Callback* next;
  Callback* prev;
  CallbackMode mode;
  CallbackType type;
  uintptr_t data;
};

void cb_free(Callback* cb);
void cb_init_cfunc(Callback* cb, const CallbackMode mode, CallbackFuncPtr func_ptr);

static inline void
cb_initfg_cfunc(Callback* cb, CallbackFuncPtr func_ptr) {
  return cb_init_cfunc(cb, kForegroundCallback, func_ptr);
}

static inline void
cb_initbg_cfunc(Callback* cb, CallbackFuncPtr func_ptr) {
  return cb_init_cfunc(cb, kBackgroundCallback, func_ptr);
}

void cb_init_lfunc(Callback* cb, const CallbackMode mode, lua_State* L, const int index);

static inline void
cb_initfg_lfunc(Callback* cb, lua_State* L, const int index) {
  return cb_init_lfunc(cb, kForegroundCallback, L, index);
}

static inline void
cb_initbg_lfunc(Callback* cb, lua_State* L, const int index) {
  return cb_init_lfunc(cb, kBackgroundCallback, L, index);
}

Callback* cb_new(CallbackMode mode, CallbackType type);

static inline Callback*
cb_newbg(CallbackType type) {
  return cb_new(kBackgroundCallback, type);
}

static inline Callback*
cb_newfg(CallbackType type) {
  return cb_new(kForegroundCallback, type);
}

static inline CallbackMode
cb_mode(Callback* cb) {
  ASSERT(cb);
  return cb->mode;
}

static inline bool
cb_is_fg(Callback* cb) {
  return cb_mode(cb) == kForegroundCallback;
}

static inline bool
cb_is_bg(Callback* cb) {
  return cb_mode(cb) == kBackgroundCallback;
}

static inline CallbackType
cb_type(Callback* cb) {
  ASSERT(cb);
  return cb->type;
}

static inline bool
cb_is_lua(Callback* cb) {
  return cb_type(cb) == kLFunc;
}

static inline bool
cb_is_func_ptr(Callback* cb) {
  return cb_type(cb) == kCFunc;
}

static inline int 
cb_get_lua_ref(Callback* cb) {
  ASSERT(cb);
  ASSERT(cb_is_lua(cb));
  return (int)cb->data;
}

static inline bool
cb_has_lua_ref(Callback* cb) {
  return cb
      && cb_is_lua(cb)
      && (cb_get_lua_ref(cb) != LUA_NOREF);
}

static inline void
cbL_push(lua_State* L, Callback* cb) {
  ASSERT(L);
  ASSERT(cb_has_lua_ref(cb));
  lua_rawgeti(L, LUA_REGISTRYINDEX, cb_get_lua_ref(cb));
}

void cbL_pcall(lua_State* L, Callback* cb);

static inline CallbackFuncPtr
cb_get_func_ptr(Callback* cb) {
  ASSERT(cb);
  ASSERT(cb_is_func_ptr(cb));
  return (CallbackFuncPtr)cb->data;
}

void cb_ccall(Callback* cb);

static inline void
cb_set_next(Callback* lhs, Callback* rhs) {
  ASSERT(lhs);
  ASSERT(rhs);
  lhs->next = rhs;
}

static inline bool
cb_has_next(Callback* cb) {
  ASSERT(cb);
  return cb->next != NULL;
}

static inline void
cb_set_prev(Callback* lhs, Callback* rhs) {
  ASSERT(lhs);
  ASSERT(rhs);
  lhs->prev = rhs;
}

static inline bool
cb_has_prev(Callback* cb) {
  ASSERT(cb);
  return cb->prev != NULL;
}

static inline void
cb_list_prepend(Callback** head, Callback* cb) {
  ASSERT(cb);
  if(*head)
    cb_set_prev((*head), cb);
  (*head) = cb;
}

static inline void
cb_list_append(Callback** head, Callback* cb) {
  ASSERT(cb);
  if((*head) == NULL) {
    (*head) = cb;
    return;
  }

  Callback* curr = (*head);
  while(cb_has_next(curr))
    curr = curr->next;
  cb_set_next(curr, cb);
  cb_set_prev(cb, curr);
}

static inline void
cb_list_insert_after(Callback* prev, Callback* cb) {
  if(!prev)
    return;
  cb_set_next(cb, prev->next);
  cb_set_next(prev, cb);
  cb_set_prev(cb, prev);
  if(cb_has_next(cb))
    cb_set_prev(cb->next, cb);
}

typedef enum {
  kForwardCallbackIter = 0,
  kBackwardCallbackIter,
} CallbackIterMode;

typedef struct _CallbackIter CallbackIter;
struct _CallbackIter {
  Callback* curr;
  CallbackIterMode mode;
};

static inline CallbackIterMode
cb_iter_mode(CallbackIter* iter) {
  ASSERT(iter);
  return iter->mode == kForwardCallbackIter;
}

static inline bool
cb_iter_is_forward(CallbackIter* iter) {
  return cb_iter_mode(iter) == kForwardCallbackIter;
}

static inline bool
cb_iter_is_backward(CallbackIter* iter) {
  return cb_iter_mode(iter) == kBackwardCallbackIter;
}

static inline bool
cb_iter_has_next(CallbackIter* iter) {
  ASSERT(iter);
  return cb_iter_is_forward(iter) ? iter->curr->next : iter->curr->prev;
}

static inline void
cb_iter_next(CallbackIter* iter) {
  ASSERT(iter);
  iter->curr = cb_iter_is_forward(iter) ? iter->curr->next : iter->curr->prev;
}

#endif // MBAR_LUA_CALLBACK_H
