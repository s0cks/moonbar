#ifndef MBAR_BUTTON_H
#define MBAR_BUTTON_H

#include "moonbar.h"

#define FOR_EACH_BUTTON_SIGNAL(V) \
  V(Clicked, clicked) \
  V(Show, show) \
  V(Hide, hide)

enum Signal : int {
#define DEFINE_SIGNAL(Name, Signal) kButton##Name##Sig,
  FOR_EACH_BUTTON_SIGNAL(DEFINE_SIGNAL)
#undef DEFINE_SIGNAL
  kTotalNumberOfButtonSignals,
};

struct _Button {
  BarApp* owner;
  GtkWidget* handle;
  int callbacks[kTotalNumberOfButtonSignals];
};

#endif // MBAR_BUTTON_H
