#ifndef MYBAR_H
#define MYBAR_H

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <luajit.h>

#include <uv.h>

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

#include "mybar/event_router.h"

typedef struct _BarApp {
  char* home;
  int argc;
  char** argv;
  EventRoute* events;
  // lua stuff
  lua_State* L;
  int config_ref;
  // gtk stuff
  GtkApplication* app;
  GtkWidget* window;
  // uv stuff
  uv_loop_t* loop;
  struct _uv_gsource* source;
} BarApp;

bool bar_app_init(BarApp* app, int argc, char** argv);
void bar_app_free(BarApp* app);
bool bar_app_run(BarApp* app);
void bar_app_load_style(BarApp* app, const char* filename);
char* bar_app_get_cwd(BarApp* app);

typedef struct _Bar Bar;
typedef struct _Label Label;
typedef struct _Button Button;

char* bar_get_version();
char* bar_get_home_from_env();

Label* bar_create_label(BarApp* app, const char* text);
void bar_free_label(Label* value);

Button* bar_create_button(BarApp* app, const char* text);
void bar_free_button(Button* value);

BarApp* barL_get_bar_app(lua_State* L);
void barL_dostring(BarApp* app, const char* code);
void barL_dofile(BarApp* app, const char* filename);

void barL_pusheventroute(lua_State* L, EventRoute* value);

void barL_pushlabel(lua_State* L, Label* value);
Label* barL_tolabel(lua_State* L, const int index);

static inline void
barL_pushlabelx(BarApp* app, Label* value) {
#define L app->L
  return barL_pushlabel(L, value);
#undef L
}

static inline Label*
barL_tolabelx(BarApp* app, const int index) {
#define L app->L
  return barL_tolabel(L, index);
#undef L
}

void barL_pushbutton(lua_State* L, Button* value);
Button* barL_tobutton(lua_State* L, const int index);

static inline void
barL_pushbuttonx(BarApp* app, Button* value) {
#define L app->L
  return barL_pushbutton(L, value);
#undef L
}

static inline Button*
barL_tobuttonx(BarApp* app, const int index) {
#define L app->L
  return barL_tobutton(L, index);
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

#ifdef BAR_DEBUG

// TODO(@s0cks):

#endif // BAR_DEBUG

#endif //MYBAR_H
