#define DEFINE_LUA_F(Name) \
  static inline int Name(lua_State* L)

BarApp* mbarL_get_mbar_app(lua_State* L);
void mbarL_dostring(BarApp* app, const char* code);
void mbarL_dofile(BarApp* app, const char* filename);

#define FOR_EACH_METATABLE_NAME(V) \
  V(Label) \
  V(Button) \
  V(EventRoute) \
  V(Box)

#define DEFINE_METATABLE_NAME(Name) \
  static const char* k##Name##MetatableName = #Name;

DEFINE_METATABLE_NAME(Widget);
FOR_EACH_METATABLE_NAME(DEFINE_METATABLE_NAME)
#undef DEFINE_METATABLE_NAME

#define DECLARE_LUA_PUSH_TYPE(Name, Type)             \
  void mbarL_push##Name(lua_State* L, Type* value);   \
  static inline void                                  \
  mbarL_push##Name##x(BarApp* app, Type* value) {     \
    return mbarL_push##Name(app->L, value);           \
  }

#define DECLARE_LUA_TO_TYPE(Name, Type)                   \
  Type* mbarL_to##Name(lua_State* L, const int index);    \
  static inline Type*                                     \
  mbarL_to##Name##x(BarApp* app, const int index) {       \
    return mbarL_to##Name(app->L, index);                 \
  }

#define DECLARE_LUA_TYPE_STACK_OPS(Name, Type) \
  DECLARE_LUA_PUSH_TYPE(Name, Type) \
  DECLARE_LUA_TO_TYPE(Name, Type)

// ╭────────╮
// │ Labels │
// ╰────────╯
DECLARE_LUA_TYPE_STACK_OPS(label, Label);

// ╭─────────╮
// │ Buttons │
// ╰─────────╯
DECLARE_LUA_TYPE_STACK_OPS(button, Button);

// ╭─────────────╮
// │ EventRoutes │
// ╰─────────────╯
DECLARE_LUA_TYPE_STACK_OPS(event_route, EventRoute);

// ╭───────╮
// │ Boxes │
// ╰───────╯
DECLARE_LUA_TYPE_STACK_OPS(box, Box);
