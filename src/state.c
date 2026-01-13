#include "mybar.h"
#include "state_lua.h"
#include "state_gtk.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

char* bar_get_home_from_env() {
  char* home = getenv("BAR_HOME");
  if(home)
    return strdup(home);
  home = getenv("XDG_CONFIG_HOME");
  if(home) {
    const char* new_home_suffix = "/bar";
    size_t home_len = strlen(home);
    size_t new_home_len = home_len + strlen(new_home_suffix) + 1;
    char* new_home = (char*)malloc(sizeof(char) * new_home_len);
    snprintf(new_home, new_home_len, "%s%s", home, new_home_suffix);
    return new_home;
  }
  home = getenv("HOME");
  if(!home) {
    fprintf(stderr, "failed to get $HOME environment variable");
    exit(1);
  }
  const char* new_home_suffix = "/.config/bar";
  size_t home_len = strlen(home);
  size_t new_home_len = home_len + strlen(new_home_suffix) + 1;
  char* new_home = (char*)malloc(sizeof(char) * new_home_len);
  snprintf(new_home, new_home_len, "%s%s", home, new_home_suffix);
  return new_home;
}

void bar_state_error(BarState* state, const char* err) {
  ASSERT(state);
  ASSERT(err);
  fprintf(stderr, "bar state error: %s\n", err);
  exit(1);
}

bool bar_state_init(BarState* state, int argc, char** argv) {
  ASSERT(state);
  state->argc = argc;
  state->argv = argv;
  state->home = bar_get_home_from_env();
  barL_init(state);
  bar_init_gtk_app(state);
  return true;
}

bool bar_state_run(BarState* state) {
  int status = g_application_run(G_APPLICATION(state->app), state->argc, state->argv);
  //TODO(@s0cks): check status
  return true;
}

void bar_state_close(BarState* state) {
  ASSERT(state);
  if(state->app)
    g_object_unref(state->app);
  barL_close(state);
  if(state->home)
    free(state->home);
}
