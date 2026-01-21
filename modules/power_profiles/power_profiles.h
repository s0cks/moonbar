#ifndef MOONBAR_POWER_PROFILES_H
#define MOONBAR_POWER_PROFILES_H

#include <gio/gio.h>
#include <stdlib.h>

static const gchar* kPowerProfilesName = "net.hadess.PowerProfiles";
static const gchar* kPowerProfilesObjectPath = "/net/hadess/PowerProfiles";
static const gchar* kPowerProfilesInterfaceName = "net.hadess.PowerProfiles";
typedef struct _PowerProfiles {
  GDBusProxy* proxy;
} PowerProfiles;

void pp_init(PowerProfiles* ctx);
void pp_free(PowerProfiles* ctx);

static inline PowerProfiles* pp_new() {
  PowerProfiles* ctx = (PowerProfiles*)malloc(sizeof(PowerProfiles));
  if (ctx)
    pp_init(ctx);
  return ctx;
}

#endif  // MOONBAR_POWER_PROFILES_H
