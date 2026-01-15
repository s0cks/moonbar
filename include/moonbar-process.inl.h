typedef struct _Process Process;
struct _Process {
  uv_process_t handle;
  uv_stdio_container_t io[3];
  uv_process_options_t options;
  uv_pipe_t out;
  uv_pipe_t err;
  int on_success;
  int on_failure;
  int on_finished;
  char** argv;
  int argc;
  BarApp* owner;
  Buffer* out_buffer;
  Buffer* err_buffer;
};

void mbar_proc_init(BarApp* app, Process* proc, char** argv, const int argc);
void mbar_proc_spawn(Process* proc);
void mbar_proc_close(Process* proc);

#define DEFINE_HAS_REF(Ref)                 \
  static inline bool                        \
  mbar_proc_has_##Ref(Process* proc) {      \
    ASSERT(proc);                           \
    return proc->Ref != LUA_NOREF;          \
  }
DEFINE_HAS_REF(on_success);
DEFINE_HAS_REF(on_failure);
DEFINE_HAS_REF(on_finished);
#undef DEFINE_HAS_REF

#define DEFINE_SET_REF(Ref)                                               \
  static inline void                                                      \
  mbar_proc_set_##Ref(Process* proc, lua_State* L, const int index) {     \
    ASSERT(proc);                                                         \
    ASSERT(!mbar_proc_has_##Ref(proc));                                   \
    ASSERT(L);                                                            \
    luaL_checktype(L, index, LUA_TFUNCTION);                              \
    lua_pushvalue(L, index);  \
    proc->Ref = luaL_ref(L, LUA_REGISTRYINDEX);                           \
    ASSERT(mbar_proc_has_##Ref(proc));                                    \
  }
DEFINE_SET_REF(on_success);
DEFINE_SET_REF(on_failure);
DEFINE_SET_REF(on_finished);
#undef DEFINE_SET_REF

#define DEFINE_CALL_REF(Ref, NumArgs)                             \
  static inline bool                                              \
  mbar_proc_pcall_##Ref(Process* proc) {                          \
    ASSERT(proc);                                                 \
    if(!mbar_proc_has_##Ref(proc))                                \
      return false;                                               \
    lua_rawgeti(proc->owner->L, LUA_REGISTRYINDEX, proc->Ref);    \
    lua_insert(proc->owner->L, -2);                               \
    return lua_pcall(proc->owner->L, NumArgs, 0, 0) == LUA_OK;    \
  }
DEFINE_CALL_REF(on_success, 1)
DEFINE_CALL_REF(on_failure, 1)
DEFINE_CALL_REF(on_finished, 0)
#undef DEFINE_CALL_REF

static inline Process*
mbar_proc_new(BarApp* app, char** argv, const int argc) {
  Process* proc = (Process*)malloc(sizeof(Process));
  if(!proc)
    return NULL;
  memset(proc, 0, sizeof(Process));
  mbar_proc_init(app, proc, argv, argc);
  return proc;
}

static inline Process*
mbar_proc_spawn_new(BarApp* app, char** argv, const int argc) {
  Process* proc = mbar_proc_new(app, argv, argc);
  if(proc)
    mbar_proc_spawn(proc);
  return proc;
}

static inline void
mbar_proc_free(Process* proc) {
  if(!proc)
    return;
  mbar_proc_close(proc);
  free(proc);
}
