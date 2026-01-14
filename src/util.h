#ifndef MBAR_UTIL_H
#define MBAR_UTIL_H

#include <unistd.h>

bool file_exists(const char* filename);
char* mbar_get_config_dir();
char* mbar_get_cwd();

#endif // MBAR_UTIL_H
