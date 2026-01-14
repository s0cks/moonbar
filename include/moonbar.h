#ifndef MOONBAR_H
#define MOONBAR_H

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <luajit.h>

#include <uv.h>

#include <gtk/gtk.h>
#include <gtk4-layer-shell.h>

#ifdef MBAR_DEBUG

#include <assert.h>

#define ASSERT(x) assert((x))

#else

#define ASSERT(x) 

#endif // MBAR_DEBUG

// clang-format off
#define MBAR_VERSION_MAJOR @PROJECT_VERSION_MAJOR@
#define MBAR_VERSION_MINOR @PROJECT_VERSION_MINOR@
#define MBAR_VERSION_PATCH @PROJECT_VERSION_PATCH@
// clang-format on

#include "moonbar/callback.h"
#include "moonbar/event_router.h"

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
  GtkCenterBox* box;
  GtkWidget* left;
  GtkWidget* center;
  GtkWidget* right;
  int update_timer;
  // uv stuff
  uv_loop_t* loop;
  Callback* next_tick_listeners;
  uv_timer_t timer;
  struct _uv_gsource* source;
} BarApp;

bool mbar_app_init(BarApp* app, int argc, char** argv);
void mbar_app_free(BarApp* app);
bool mbar_app_run(BarApp* app);
void mbar_app_load_style(BarApp* app, const char* filename);
char* mbar_app_get_cwd(BarApp* app);

typedef struct _Bar Bar;
typedef struct _Label Label;
typedef struct _Button Button;

char* mbar_get_version();
char* mbar_get_home_from_env();

Label* mbar_create_label(BarApp* app, const char* text);
void mbar_free_label(Label* value);

Button* mbar_create_button(BarApp* app, const char* text);
void mbar_free_button(Button* value);

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

#ifdef MBAR_DEBUG

// TODO(@s0cks):

#endif // MBAR_DEBUG

#endif //MOONBAR_H
