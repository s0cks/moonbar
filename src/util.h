#ifndef BAR_UTIL_H
#define BAR_UTIL_H

#include <unistd.h>

static inline bool
file_exists(const char* filename) {
  ASSERT(filename);
  return access(filename, F_OK) == 0;
}

#endif // BAR_UTIL_H
