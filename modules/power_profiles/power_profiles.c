#include "power_profiles.h"

#include <gio/gio.h>
#include <stdio.h>

void pp_init(PowerProfiles* ctx) {
  GError* err = nullptr;
  // clang-format off
  ctx->proxy = g_dbus_proxy_new_for_bus_sync(G_BUS_TYPE_SYSTEM, 
                                             G_DBUS_PROXY_FLAGS_NONE, 
                                             nullptr, 
                                             kPowerProfilesName,
                                             kPowerProfilesObjectPath, 
                                             kPowerProfilesInterfaceName, 
                                             nullptr, 
                                             &err);
  // clang-format on
  if (err) {
    fprintf(stderr, "error creating dbus proxy: %s\n", err->message);
    g_error_free(err);
    return;
  }
}

void pp_free(PowerProfiles* ctx) {
  if (!ctx)
    return;
  g_object_unref(ctx->proxy);
}
