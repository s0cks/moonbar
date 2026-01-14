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
