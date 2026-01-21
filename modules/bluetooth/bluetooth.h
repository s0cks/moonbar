#ifndef MOONBAR_BLUETOOTH_H
#define MOONBAR_BLUETOOTH_H

#include <gio/gio.h>

#include "moonbar.h"

typedef struct _BleState {
  GDBusProxy* proxy;
} BleState;

void bles_init(BleState* ble);
bool bles_is_powered(BleState* ble);
void bles_free(BleState* ble);

// clang-format off
#define DECLARE_BLES_STRING_GETTER(Name) \
  const char* bles_get_##Name(BleState* ble)

DECLARE_BLES_STRING_GETTER(name);
DECLARE_BLES_STRING_GETTER(address);
DECLARE_BLES_STRING_GETTER(address_type);
DECLARE_BLES_STRING_GETTER(alias);
#undef DECLARE_BLES_STRING_GETTER
// clang-format on

static inline BleState* bles_new() {
  BleState* ble = (BleState*)malloc(sizeof(BleState));
  if (ble)
    bles_init(ble);
  return ble;
}

static const gchar* kBluezName = "org.bluez";
static const gchar* kBluezObjectPath = "/org/bluez/hci0";
static const gchar* kBluezInterfaceName = "org.freedesktop.DBus.Properties";
static const gchar* kBluezAdapter1 = "org.bluez.Adapter1";
static const gchar* kBluezPoweredField = "Powered";
static const gchar* kBluezAddressField = "Address";
static const gchar* kBluezAddressTypeField = "AddressType";
static const gchar* kBluezNameField = "Name";
static const gchar* kBluezAliasField = "Alias";

#endif  // MOONBAR_BLUETOOTH_H
