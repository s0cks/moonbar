#include "upowerctl.h"

static const gchar* kUPowerctlName = "org.freedesktop.UPower";
static const gchar* kUPowerctlObjectPath = "/org/freedesktop/UPower/KbdBacklight";
static const gchar* kUPowerctlInterfaceName = "org.freedesktop.UPower.KbdBacklight";

void upow_init(UPowerctl* ctx) {
  GError* err = nullptr;
  // clang-format off
  ctx->proxy = g_dbus_proxy_new_for_bus_sync(G_BUS_TYPE_SYSTEM,
                                             G_DBUS_PROXY_FLAGS_NONE,
                                             nullptr,
                                             kUPowerctlName,
                                             kUPowerctlObjectPath,
                                             kUPowerctlInterfaceName,
                                             nullptr,
                                             &err);
  // clang-format on
  if (err) {
    fprintf(stderr, "failed to create dbus UPowerctl: %s\n", err->message);
    g_error_free(err);
    return;
  }
}

bool upow_get(UPowerctl* ctx, upow_brightness_t* result) {
  bool success = false;
  if (!ctx || !ctx->proxy)
    goto finished;

  GError* err = nullptr;
  // clang-format off
  GVariant* var = g_dbus_proxy_call_sync(ctx->proxy, 
                                         "GetBrightness", 
                                         nullptr,
                                         G_DBUS_CALL_FLAGS_NONE, 
                                         -1, 
                                         nullptr, 
                                         &err);
  // clang-format on
  if (err) {
    fprintf(stderr, "failed to get keyboard brightness using upower (dbus): %s\n", err->message);
    g_error_free(err);
    goto finished;
  }

  if (var) {
    g_variant_get(var, "(i)", result);
    g_variant_unref(var);
    success = true;
  }
finished:
  return success;
}

bool upow_get_max(UPowerctl* ctx, upow_brightness_t* result) {
  bool success = false;
  if (!ctx || !ctx->proxy)
    goto finished;

  GError* err = nullptr;
  // clang-format off
  GVariant* var = g_dbus_proxy_call_sync(ctx->proxy, 
                                         "GetMaxBrightness", 
                                         nullptr,
                                         G_DBUS_CALL_FLAGS_NONE, 
                                         -1, 
                                         nullptr, 
                                         &err);
  // clang-format on
  if (err) {
    fprintf(stderr, "failed to get keyboard brightness using upower (dbus): %s\n", err->message);
    g_error_free(err);
    goto finished;
  }

  if (var) {
    g_variant_get(var, "(i)", result);
    g_variant_unref(var);
    success = true;
  }
finished:
  return success;
}

bool upow_set(UPowerctl* ctx, const upow_brightness_t value) {
  bool success = false;
  if (!ctx || !ctx->proxy)
    goto finished;
  GError* err = nullptr;
  // clang-format off
  g_dbus_proxy_call_sync(ctx->proxy, 
                         "SetBrightness", 
                         g_variant_new("(i)", 3), 
                         G_DBUS_CALL_FLAGS_NONE, 
                         -1, 
                         nullptr, 
                         &err);
  // clang-format on
  if (err) {
    fprintf(stderr, "failed to set keyboard brightness using upower (dbus): %s\n", err->message);
    g_error_free(err);
    goto finished;
  }

  success = true;
finished:
  return success;
}

void upow_free(UPowerctl* ctx) {
  if (!ctx)
    return;
  if (ctx->proxy)
    g_object_unref(ctx->proxy);
}
