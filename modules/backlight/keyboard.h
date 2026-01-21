#ifndef MOONBAR_BACKLIGHT_KEYBOARD_H
#define MOONBAR_BACKLIGHT_KEYBOARD_H

#include "moonbar.h"
#include "upowerctl.h"

static const char* kKeyboardMetatableName = "Keyboard";
void init_keyboard_metatable(lua_State* L);

#endif  // MOONBAR_BACKLIGHT_KEYBOARD_H
