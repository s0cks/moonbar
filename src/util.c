#include "util.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "moonbar.h"

bool file_exists(const char* filename) {
  ASSERT(filename);
  return access(filename, F_OK) == 0;
}

char* mbar_get_config_dir() {
  char* home = getenv("MBAR_HOME");
  if (home)
    return strdup(home);
  home = getenv("XDG_CONFIG_HOME");
  if (home) {
    const char* new_home_suffix = "/bar";
    size_t home_len = strlen(home);
    size_t new_home_len = home_len + strlen(new_home_suffix) + 1;
    char* new_home = (char*)malloc(sizeof(char) * new_home_len);
    snprintf(new_home, new_home_len, "%s%s", home, new_home_suffix);
    return new_home;
  }
  home = getenv("HOME");
  if (!home) {
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

char* mbar_get_cwd() {
  char cwd[PATH_MAX];
  if (getcwd(cwd, sizeof(cwd)) == NULL) {
    fprintf(stderr, "failed to get current working directory.\n");
    return nullptr;
  }
  return strdup(cwd);
}
