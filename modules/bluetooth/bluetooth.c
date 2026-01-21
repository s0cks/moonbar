#include "bluetooth.h"

#include <gio/gio.h>
#include <glib.h>

void bles_init(BleState* ble) {
  if (!ble)
    return;
  GError* err = nullptr;
  GDBusProxy* proxy = g_dbus_proxy_new_for_bus_sync(G_BUS_TYPE_SYSTEM, G_DBUS_PROXY_FLAGS_NONE, nullptr, kBluezName,
                                                    kBluezObjectPath, kBluezInterfaceName, nullptr, &err);
  if (err) {
    fprintf(stderr, "error creating bluez dbus proxy: %s\n", err->message);
    g_error_free(err);
    if (proxy)
      g_object_unref(proxy);
    return;
  }
  ble->proxy = proxy;
}

static inline bool bles_get_field_str(BleState* ble, const gchar* field, const gchar** result, size_t* result_len) {
  GVariant* var = nullptr;
  GError* err = nullptr;
  if (ble) {
    var = g_variant_new("(ss)", kBluezAdapter1, field);
    var = g_dbus_proxy_call_sync(ble->proxy, "Get", var, G_DBUS_CALL_FLAGS_NONE, -1, nullptr, &err);
    if (err) {
      fprintf(stderr, "error getting %s property: %s\n", field, err->message);
      g_error_free(err);
      goto finished;
    }

    g_variant_get(var, "(v)", &var);

    size_t data_len = 0;
    const gchar* data = g_variant_get_string(var, &data_len);
    if (!data || data_len <= 0) {
      fprintf(stderr, "failed to get %s property data\n", field);
      goto finished;
    }

    (*result) = data;
    (*result_len) = data_len;
  }

finished:
  if (var)
    g_variant_unref(var);
  return (*result_len) > 0;
}

const char* bles_get_address(BleState* ble) {
  const char* result = nullptr;
  size_t result_len = 0;
  if (!bles_get_field_str(ble, kBluezAddressField, &result, &result_len))
    return nullptr;
  return result;
}

const char* bles_get_address_type(BleState* ble) {
  const char* result = nullptr;
  size_t result_len = 0;
  if (!bles_get_field_str(ble, kBluezAddressTypeField, &result, &result_len))
    return nullptr;
  return result;
}

const char* bles_get_name(BleState* ble) {
  const char* result = nullptr;
  size_t result_len = 0;
  if (!bles_get_field_str(ble, kBluezNameField, &result, &result_len))
    return nullptr;
  return result;
}

const char* bles_get_alias(BleState* ble) {
  const char* result = nullptr;
  size_t result_len = 0;
  if (!bles_get_field_str(ble, kBluezAliasField, &result, &result_len))
    return nullptr;
  return result;
}

static inline bool bles_get_field_bool(BleState* ble, const gchar* field) {
  GVariant* var = nullptr;
  GError* err = nullptr;
  bool state = false;
  if (ble) {
    var = g_variant_new("(ss)", kBluezAdapter1, field);
    var = g_dbus_proxy_call_sync(ble->proxy, "Get", var, G_DBUS_CALL_FLAGS_NONE, -1, nullptr, &err);
    if (err) {
      fprintf(stderr, "error getting %s property: %s\n", field, err->message);
      g_error_free(err);
      goto finished;
    }

    g_variant_get(var, "(v)", &var);
    state = g_variant_get_boolean(var);
  }

finished:
  if (var)
    g_variant_unref(var);
  return state;
}

bool bles_is_powered(BleState* ble) {
  return bles_get_field_bool(ble, kBluezPoweredField);
}

// ble properties:
//  - Class
// uint32 7077888
//  - Connectable
// false
//  - Powered
// true
//  - PowerState
// 'on'
//  - Discoverable
// false
//  - DiscoverableTimeout
// uint32 180
//  - Pairable
// false
//  - PairableTimeout
// uint32 0
//  - Discovering
// false
//  - UUIDs
// ['0000110e-0000-1000-8000-00805f9b34fb', '0000111f-0000-1000-8000-00805f9b34fb',
// '00001200-0000-1000-8000-00805f9b34fb', '0000110b-0000-1000-8000-00805f9b34fb',
// '0000110a-0000-1000-8000-00805f9b34fb', '0000110c-0000-1000-8000-00805f9b34fb',
// '00001800-0000-1000-8000-00805f9b34fb', '00001801-0000-1000-8000-00805f9b34fb',
// '0000180a-0000-1000-8000-00805f9b34fb', '03b80e5a-ede8-4b33-a751-6ce34ec4c700',
// '0000111e-0000-1000-8000-00805f9b34fb']
//  - Modalias
// 'usb:v1D6Bp0246d0555'
//  - Roles
// ['central', 'peripheral']
//  - Manufacturer
// uint16 2
//  - Version
// byte 0x0d

void bles_free(BleState* ble) {
  if (!ble)
    return;
  g_object_unref(ble->proxy);
}
