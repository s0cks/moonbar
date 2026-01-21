#include <gio/gio.h>
#include <glib.h>

#include "moonbar.h"
#include "power_profiles.h"

static PowerProfiles* instance = nullptr;

#define check_instance(L)                                     \
  if (!instance) {                                            \
    instance = pp_new();                                      \
    if (!instance) {                                          \
      luaL_error(L, "failed to create power-profiles state"); \
      return 0;                                               \
    }                                                         \
  }

DEFINE_LUA_F(pp_get_profiles) {
  check_instance(L);

  GError* err = nullptr;
  GVariantIter iter;
  GVariant* profile = nullptr;

  lua_newtable(L);

  GVariant* profiles = g_dbus_proxy_get_cached_property(instance->proxy, "Profiles");
  if (profiles) {
    fprintf(stdout, "getting power profiles...\n");
    g_variant_iter_init(&iter, profiles);
    while (g_variant_iter_next(&iter, "@a{sv}", &profile)) {
      fprintf(stdout, "getting power profile name...\n");
      lua_newtable(L);
      const gchar* name = nullptr;
      if (!g_variant_lookup(profile, "Profile", "&s", &name)) {
        luaL_error(L, "failed to get power profile name");
        return 0;
      }
      lua_pushstring(L, (const char*)name);
      lua_setfield(L, -2, "name");

      fprintf(stdout, "getting power profile driver...\n");
      const gchar* driver = nullptr;
      if (!g_variant_lookup(profile, "Driver", "&s", &driver)) {
        luaL_error(L, "failed to get power profile driver");
        return 0;
      }
      lua_pushstring(L, (const char*)driver);
      lua_setfield(L, -2, "driver");

      g_variant_unref(profile);

      const int index = (int)(lua_objlen(L, -2) + 1);
      lua_rawseti(L, -2, index);
    }
    g_variant_unref(profiles);
  } else {
    fprintf(stdout, "no profiles\n");
  }
  fprintf(stdout, "done.\n");
  return 1;
}

DEFINE_LUA_F(pp_get_active) {
  check_instance(L);
  GError* err = nullptr;
  // clang-format off
  GVariant* var = g_dbus_proxy_call_sync(instance->proxy,
                                         "org.freedesktop.DBus.Properties.Get",
                                         g_variant_new("(ss)", kPowerProfilesInterfaceName, "ActiveProfile"),
                                         G_DBUS_CALL_FLAGS_NONE,
                                         -1,
                                         nullptr,
                                         &err);
  // clang-format on
  if (err) {
    luaL_error(L, "failed to get ActiveProfiles");
    g_error_free(err);
    return 0;
  }

  if (var) {
    GVariant* inner = nullptr;
    g_variant_get(var, "(v)", &inner);
    g_variant_unref(var);
    var = inner;
  }

  if (!var) {
    luaL_error(L, "failed to get ActiveProfiles variable");
    return 0;
  }

  gchar* active = g_variant_dup_string(var, nullptr);
  lua_pushstring(L, (const char*)active);
  g_free(active);

  g_variant_unref(var);
  return 1;
}

DEFINE_LUA_F(pp_close) {
  check_instance(L);
  pp_free(instance);
  return 0;
}

DEFINE_LUA_F(pp_set_active) {
  check_instance(L);
  const char* new_profile = lua_tostring(L, 1);

  GError* err = nullptr;
  // clang-format off
  GVariant* var = g_dbus_proxy_call_sync(instance->proxy,
                                         "org.freedesktop.DBus.Properties.Set",
                                         g_variant_new("(ssv)", kPowerProfilesInterfaceName, "ActiveProfile", g_variant_new_string(new_profile)),
                                         G_DBUS_CALL_FLAGS_NONE,
                                         -1,
                                         nullptr,
                                         &err);
  // clang-format on
  if (err) {
    luaL_error(L, "failed to set ActiveProfile to '%s': %s", new_profile, err->message);
    g_error_free(err);
    goto finished;
  }

finished:
  if (var)
    g_variant_unref(var);
  return 0;
}

// clang-format off
static const luaL_Reg kLibFuncs[] = {
#define BIND(Name) \
  { #Name, pp_##Name }

  BIND(close),
  BIND(get_profiles),
  BIND(get_active),
  BIND(set_active),
#undef BIND
  {nullptr, nullptr},
};
// clang-format on

DEFINE_LUAOPEN_MOONBAR_LIB_F(power_profiles);
