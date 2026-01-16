#ifndef MBAR_BOX_H
#define MBAR_BOX_H

#include "moonbar.h"
#include "widget.h"

#define FOR_EACH_BOX_SIGNAL(V) \
  FOR_EACH_WIDGET_SIGNAL(V)

struct _Box {
  MOONBAR_WIDGET_FIELDS;
};

#endif // MBAR_BOX_H
