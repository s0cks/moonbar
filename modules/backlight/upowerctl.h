#ifndef MOONBAR_UPOWERCTL_H
#define MOONBAR_UPOWERCTL_H

#include "moonbar.h"

typedef uint64_t upow_brightness_t;

typedef struct _UPowerctl {
  GDBusProxy* proxy;
} UPowerctl;

void upow_init(UPowerctl* ctx);
void upow_free(UPowerctl* ctx);

bool upow_set(UPowerctl* ctx, const upow_brightness_t value);
bool upow_get_max(UPowerctl* ctx, upow_brightness_t* value);
bool upow_get(UPowerctl* ctx, upow_brightness_t* value);

static inline UPowerctl* upow_new() {
  UPowerctl* ctx = (UPowerctl*)malloc(sizeof(UPowerctl));
  if (ctx)
    upow_init(ctx);
  return ctx;
}

#endif  // MOONBAR_UPOWERCTL_H
