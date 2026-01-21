#include "bluetooth.h"

static BleState* instance = nullptr;

#define check_instance(L)                          \
  if (!instance) {                                 \
    instance = bles_new();                         \
    if (!instance) {                               \
      luaL_error(L, "failed to create ble state"); \
      return 0;                                    \
    }                                              \
  }

DEFINE_LUA_F(is_powered) {
  check_instance(L);
  lua_pushboolean(L, bles_is_powered(instance));
  return 1;
}

// TODO(@s0cks): do we need to free address?
#define DEFINE_LUA_BLE_STRING_PROPERTY_GETTER_F(Name) \
  DEFINE_LUA_F(get_##Name) {                          \
    check_instance(L);                                \
    const char* value = bles_get_##Name(instance);    \
    if (!value) {                                     \
      lua_pushnil(L);                                 \
    } else {                                          \
      lua_pushstring(L, value);                       \
    }                                                 \
    return 1;                                         \
  }

DEFINE_LUA_BLE_STRING_PROPERTY_GETTER_F(name);
DEFINE_LUA_BLE_STRING_PROPERTY_GETTER_F(alias);
DEFINE_LUA_BLE_STRING_PROPERTY_GETTER_F(address);
DEFINE_LUA_BLE_STRING_PROPERTY_GETTER_F(address_type);

#ifdef _NDEBUG

static inline void print_variant(GVariant* v) {
  gchar* s = g_variant_print(v, TRUE);
  fprintf(stdout, "%s\n", s);
  g_free(s);
}

DEFINE_LUA_F(print_properties) {
  check_instance(L);
  GError* err = nullptr;
  GVariantIter* iter = nullptr;
  gchar* key = nullptr;
  GVariant* value = nullptr;

  GVariant* var = g_dbus_proxy_call_sync(instance->proxy, "GetAll", g_variant_new("(s)", kBluezAdapter1),
                                         G_DBUS_CALL_FLAGS_NONE, -1, nullptr, &err);
  if (err) {
    fprintf(stderr, "failed to get all properties: %s\n", err->message);
    g_error_free(err);
    return 0;
  }

  g_variant_get(var, "(a{sv})", &iter);
  fprintf(stdout, "ble properties:\n");
  while (g_variant_iter_next(iter, "{sv}", &key, &value)) {
    fprintf(stdout, " - %s\n", key);
    print_variant(value);
    g_free(key);
    g_variant_unref(value);
  }
  g_variant_iter_free(iter);
  g_variant_unref(var);
  return 0;
}

#endif

// clang-format off
static const luaL_Reg kLibFuncs[] = {
  {"is_powered", is_powered},
#define DEFINE_GETTER(Name) \
  { "get_" #Name, get_##Name}

  DEFINE_GETTER(name),
  DEFINE_GETTER(alias),
  DEFINE_GETTER(address),
  DEFINE_GETTER(address_type),

#undef DEFINE_GETTER

#ifdef _NDEBUG
  { "print_properties", print_properties },
#endif //_NDEBUG
  {nullptr, nullptr},
};
// clang-format on

DEFINE_LUAOPEN_MOONBAR_LIB_F(bluetooth);
