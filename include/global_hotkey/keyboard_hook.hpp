#ifndef GLOBAL_HOTKEY_KEYBOARD_HOOK_HPP
#define GLOBAL_HOTKEY_KEYBOARD_HOOK_HPP

#include "core/core.hpp"

// Only usable in windows platform.
#ifdef GLOBAL_HOTKEY_WIN

namespace gbhk
{

namespace keyboard_hook
{

enum State
{
    PRESSED,
    RELEASED
};

void addKeyEventCallback(int key, State state, VoidFunc callbackFunc);

void addKeyEventCallback(int key, State state, ArgFunc callbackFunc, Arg arg);

void setKeyPressedCallback(void (*callbackFunc)(int key));

void setKeyReleaseddCallback(void (*callbackFunc)(int key));

/// @return If success return #RC_SUCCESS, else return error code.
int run();

/// @return If success return #RC_SUCCESS, else return error code.
int end();

} // namespace keyboard_hook

} // namespace gbhk

#endif // GLOBAL_HOTKEY_WIN

#endif // !GLOBAL_HOTKEY_KEYBOARD_HOOK_HPP
