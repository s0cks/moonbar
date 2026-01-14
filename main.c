#include "moonbar.h"

int main(int argc, char **argv) {
  BarApp app;
  if(!mbar_app_init(&app, argc, argv))
    return EXIT_FAILURE;
  mbar_app_run(&app);
  mbar_app_free(&app);
  return EXIT_SUCCESS;
}

