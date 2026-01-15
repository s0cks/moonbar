static inline bool
mbar_has_config(BarApp* app) {
  ASSERT(app);
  return app->config_ref != LUA_NOREF;
}

const char* mbar_config_gets_or(BarApp* app, const char* name, const char* default_value);

static inline const char*
mbar_config_gets(BarApp* app, const char* name) {
  return mbar_config_gets_or(app, name, NULL);
}

bool mbar_config_getb_or(BarApp* app, const char* name, const bool default_value);

static inline bool
mbar_config_getb(BarApp* app, const char* name) {
  return mbar_config_getb_or(app, name, false);
}

int mbar_config_geti_or(BarApp* app, const char* name, const int default_value);

static inline int
mbar_config_geti(BarApp* app, const char* name) {
  return mbar_config_geti_or(app, name, 0);
}


#define MBAR_STYLE "style"
#define MBAR_DEFAULT_STYLE "default.css"
static inline const char*
mbar_config_get_style(BarApp* app) {
  return mbar_config_gets_or(app, MBAR_STYLE, MBAR_DEFAULT_STYLE);
}

#define MBAR_WATCH_STYLE "watch_style"
#define MBAR_DEFAULT_WATCH_STYLE true
static inline bool
mbar_config_get_watch_style(BarApp* app) {
  return mbar_config_getb_or(app, MBAR_WATCH_STYLE, MBAR_DEFAULT_WATCH_STYLE);
}
