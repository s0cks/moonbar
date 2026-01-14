BarApp* mbarL_get_mbar_app(lua_State* L);
void mbarL_dostring(BarApp* app, const char* code);
void mbarL_dofile(BarApp* app, const char* filename);

void mbarL_pusheventroute(lua_State* L, EventRoute* value);

void mbarL_pushlabel(lua_State* L, Label* value);
Label* mbarL_tolabel(lua_State* L, const int index);

static inline void
mbarL_pushlabelx(BarApp* app, Label* value) {
#define L app->L
  return mbarL_pushlabel(L, value);
#undef L
}

static inline Label*
mbarL_tolabelx(BarApp* app, const int index) {
#define L app->L
  return mbarL_tolabel(L, index);
#undef L
}

void mbarL_pushbutton(lua_State* L, Button* value);
Button* mbarL_tobutton(lua_State* L, const int index);

static inline void
mbarL_pushbuttonx(BarApp* app, Button* value) {
#define L app->L
  return mbarL_pushbutton(L, value);
#undef L
}

static inline Button*
mbarL_tobuttonx(BarApp* app, const int index) {
#define L app->L
  return mbarL_tobutton(L, index);
#undef L
}

#define FOR_EACH_METATABLE_NAME(V) \
  V(Label) \
  V(Button) \
  V(EventRoute)

#define DEFINE_METATABLE_NAME(Name) \
  static const char* k##Name##MetatableName = #Name;

FOR_EACH_METATABLE_NAME(DEFINE_METATABLE_NAME)
#undef DEFINE_METATABLE_NAME
