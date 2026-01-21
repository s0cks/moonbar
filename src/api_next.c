#include "api.h"
#include "app.h"
#include "file_line_reader.h"
#include "hypr_client.h"
#include "log.h"
#include "moonbar.h"
#include "state_gtk.h"
#include "state_lua.h"
#include "util.h"
#include "uv_gsource.h"

int mbarL_api_next(lua_State* L) {
  mbarL_check_global_app(L, app);
  Callback* cb = (Callback*)malloc(sizeof(Callback));
  if (!cb) {
    luaL_error(L, "failed to create Callback");
    return 0;
  }
  cb_initbg_lfunc(cb, L, 1);
  cb_list_append(&app->next_tick_listeners, cb);
  return 0;
}
