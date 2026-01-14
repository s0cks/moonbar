typedef struct _BarApp {
  char* home;
  int argc;
  char** argv;
  EventRoute* events;
  // lua stuff
  lua_State* L;
  int config_ref;
  // gtk stuff
  GtkApplication* app;
  GtkWidget* window;
  GtkCenterBox* box;
  //TODO(@s0cks): convert to GtkBox
  GtkWidget* left;
  GtkWidget* center;
  GtkWidget* right;
  int update_timer;
  // uv stuff
  uv_loop_t* loop;
  Callback* next_tick_listeners;
  uv_timer_t timer;
  struct _uv_gsource* source;
} BarApp;

bool mbar_app_init(BarApp* app, int argc, char** argv);
void mbar_app_free(BarApp* app);
bool mbar_app_run(BarApp* app);
void mbar_app_load_style(BarApp* app, const char* filename);
char* mbar_app_get_cwd(BarApp* app);
char* mbar_get_home_from_env();

#define DEFINE_BOX_GET(Side)            \
  static inline GtkBox*                 \
  mbar_get_##Side(BarApp* app) {        \
    ASSERT(app);                        \
    return GTK_BOX(app->Side);          \
  }
DEFINE_BOX_GET(left);
DEFINE_BOX_GET(center);
DEFINE_BOX_GET(right);
#undef DEFINE_BOX_GET

#define DEFINE_BOX_APPEND(Side)                             \
  static inline void                                        \
  mbar_append_##Side(BarApp* app, GtkWidget* widget) {      \
    ASSERT(app);                                            \
    ASSERT(widget);                                         \
    gtk_box_append(mbar_get_##Side(app), widget);           \
  }
DEFINE_BOX_APPEND(left);
DEFINE_BOX_APPEND(center);
DEFINE_BOX_APPEND(right);
#undef DEFINE_BOX_APPEND

#define DEFINE_BOX_PREPEND(Side)                            \
  static inline void                                        \
  mbar_prepend_##Side(BarApp* app, GtkWidget* widget) {     \
    ASSERT(app);                                            \
    ASSERT(widget);                                         \
    gtk_box_prepend(mbar_get_##Side(app), widget);          \
  }
DEFINE_BOX_PREPEND(left);
DEFINE_BOX_PREPEND(center);
DEFINE_BOX_PREPEND(right);
#undef DEFINE_BOX_PREPEND
