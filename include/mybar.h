#ifndef MYBAR_H
#define MYBAR_H

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <luajit.h>

#include <gtk/gtk.h>
#include <gtk4-layer-shell.h>

#ifdef BAR_DEBUG

#include <assert.h>

#define ASSERT(x) assert((x))

#else

#define ASSERT(x) 

#endif // BAR_DEBUG

// clang-format off
#define BAR_VERSION_MAJOR @PROJECT_VERSION_MAJOR@
#define BAR_VERSION_MINOR @PROJECT_VERSION_MINOR@
#define BAR_VERSION_PATCH @PROJECT_VERSION_PATCH@
// clang-format on

typedef struct _EventRouter EventRouter;

EventRouter* evr_new();
void evr_on(lua_State* L, EventRouter* evr, const char* event, const int index);
void evr_publish(lua_State* L, EventRouter* evr, const char* event);
void evr_remove(lua_State* L, EventRouter* evr, const char* event);
void evr_free(EventRouter*);

typedef struct {
  char* home;
  int argc;
  char** argv;
  EventRouter* events;
  // lua stuff
  lua_State* L;
  // gtk stuff
  GtkApplication* app;
  GtkWidget* window;
} BarState;

typedef struct _Label Label;
typedef struct _Button Button;

char* bar_get_version();
char* bar_get_home_from_env();
bool bar_state_init(BarState* state, int argc, char** argv);
bool bar_state_run(BarState* state);
void bar_state_close(BarState* state);
void bar_load_style(BarState* state, const char* filename);

Label* bar_create_label(BarState* state, const char* text);
void bar_free_label(Label* value);

Button* bar_create_button(BarState* state, const char* text);
void bar_free_button(Button* value);

static inline void
bar_publish(BarState* state, const char* event) {
  ASSERT(state);
  ASSERT(event);
}

BarState* barL_get_bar_state(lua_State* L);
void barL_dostring(BarState* state, const char* code);
void barL_dofile(BarState* state, const char* filename);
void barL_pushevr(lua_State* state, EventRouter* evr);
EventRouter* barL_getevr(lua_State* state, const int index);

static inline void
barL_pushevrx(BarState* state, EventRouter* evr) {
#define L state->L
  return barL_pushevr(L, evr);
#undef L
}

static inline EventRouter*
barL_getevrx(BarState* state, const int index) {
#define L state->L
  return barL_getevr(L, index);
#undef L
}

void barL_pushlabel(lua_State* L, Label* value);
Label* barL_tolabel(lua_State* L, const int index);

static inline void
barL_pushlabelx(BarState* state, Label* value) {
#define L state->L
  return barL_pushlabel(L, value);
#undef L
}

static inline Label*
barL_tolabelx(BarState* state, const int index) {
#define L state->L
  return barL_tolabel(L, index);
#undef L
}

void barL_pushbutton(lua_State* L, Button* value);
Button* barL_tobutton(lua_State* L, const int index);

static inline void
barL_pushbuttonx(BarState* state, Button* value) {
#define L state->L
  return barL_pushbutton(L, value);
#undef L
}

static inline Button*
barL_tobuttonx(BarState* state, const int index) {
#define L state->L
  return barL_tobutton(L, index);
#undef L
}

#define FOR_EACH_METATABLE_NAME(V) \
  V(EventRouter) \
  V(Label) \
  V(Button)

#define DEFINE_METATABLE_NAME(Name) \
  static const char* k##Name##MetatableName = #Name;

FOR_EACH_METATABLE_NAME(DEFINE_METATABLE_NAME)
#undef DEFINE_METATABLE_NAME

#ifdef BAR_DEBUG

// TODO(@s0cks):

#endif // BAR_DEBUG

#endif //MYBAR_H
