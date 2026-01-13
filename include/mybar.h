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

typedef struct {
  char* home;
  int argc;
  char** argv;
  // lua stuff
  lua_State* L;
  // gtk stuff
  GtkApplication* app;
  GtkWidget* window;
} BarState;

char* bar_get_version();
char* bar_get_home_from_env();
bool bar_state_init(BarState* state, int argc, char** argv);
bool bar_state_run(BarState* state);
void bar_state_close(BarState* state);

void bar_load_style(BarState* state, const char* filename);

// barL
BarState* barL_get_bar_state(lua_State* L);
void barL_dostring(BarState* state, const char* code);
void barL_dofile(BarState* state, const char* filename);

#ifdef BAR_DEBUG

// TODO(@s0cks):

#endif // BAR_DEBUG

#endif //MYBAR_H
