#include "mybar.h"

int main(int argc, char **argv) {
  BarState state;
  if(!bar_state_init(&state, argc, argv))
    return EXIT_FAILURE;
  bar_state_run(&state);
  bar_state_close(&state);
  return EXIT_SUCCESS;
}

