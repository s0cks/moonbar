#ifndef MOONBAR_H
#define MOONBAR_H

#include <uv.h>

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <luajit.h>

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

char* mbar_get_version();
#include "moonbar-callback.inl.h"
#include "moonbar-events.inl.h"
#include "moonbar-app.inl.h"
#include "moonbar-label.inl.h"
#include "moonbar-button.inl.h"
#include "moonbar-lua.inl.h"
#include "moonbar-config.inl.h"

#ifdef MBAR_DEBUG

#include "moonbar-debug.inl.h"

#endif // MBAR_DEBUG

#endif //MOONBAR_H
