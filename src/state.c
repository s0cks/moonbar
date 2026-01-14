#include "mybar.h"
#include "state_lua.h"
#include "state_gtk.h"
#include "app.h"

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

char* bar_app_get_cwd(BarApp* app) {
  ASSERT(app);
  char cwd[PATH_MAX];
  if(getcwd(cwd, sizeof(cwd)) == NULL) {
      bar_error(app, "failed to get cwd");
      return NULL;
  }
  return strdup(cwd);
}
