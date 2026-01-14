#ifndef BAR_BUTTON_H
#define BAR_BUTTON_H

#include "mybar.h"

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

#endif // BAR_BUTTON_H
