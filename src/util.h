#ifndef MBAR_UTIL_H
#define MBAR_UTIL_H

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

bool file_exists(const char* filename);
char* mbar_get_config_dir();
char* mbar_get_cwd();

static inline char* ltrim(char* s) {
  while(isspace(*s)) s++;
  return s;
}

static inline char* rtrim(char* s) {
  char* back = s + strlen(s);
  while(isspace(*--back));
  *(back+1) = '\0';
  return s;
}

static inline char* trim(char* s) {
  return rtrim(ltrim(s)); 
}

#endif // MBAR_UTIL_H
