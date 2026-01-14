#ifndef MBAR_LABEL_H
#define MBAR_LABEL_H

#include "moonbar.h"

static constexpr const int kMaxNumberOfCallbacks = 4;

struct _Label {
  GtkWidget* handle;
  BarApp* owner;
  int callbacks[kMaxNumberOfCallbacks];
};

#endif // MBAR_LABEL_H
