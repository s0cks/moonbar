#include "mybar.h"

int main(int argc, char **argv) {
  BarApp app;
  if(!bar_app_init(&app, argc, argv))
    return EXIT_FAILURE;
  bar_app_run(&app);
  bar_app_free(&app);
  return EXIT_SUCCESS;
}

