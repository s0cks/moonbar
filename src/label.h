#ifndef BAR_LABEL_H
#define BAR_LABEL_H

#include "mybar.h"

static constexpr const int kMaxNumberOfCallbacks = 4;

struct _Label {
  BarState* owner;
  GtkWidget* handle;
  int callbacks[kMaxNumberOfCallbacks];
};

#endif // BAR_LABEL_H
