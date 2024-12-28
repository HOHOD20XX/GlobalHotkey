#ifndef GLOBAL_HOTKEY_KEYBOARD_HOOK_HPP
#define GLOBAL_HOTKEY_KEYBOARD_HOOK_HPP

#include "core/core.hpp"

// Only usable in windows platform.
#ifdef GLOBAL_HOTKEY_WIN

namespace gbhk
{

namespace keyboard_hook
{

enum State : uchar
{
    PRESSED,
    RELEASED
};

void addKeyEventCallback(uint key, State state, VoidFunc callbackFunc);

void addKeyEventCallback(uint key, State state, ArgFunc callbackFunc, Arg arg);

void setKeyPressedCallback(void (*callbackFunc)(uint key));

void setKeyReleaseddCallback(void (*callbackFunc)(uint key));

/// @return If success return #RC_SUCCESS, else return error code.
uint run();

/// @return If success return #RC_SUCCESS, else return error code.
uint end();

} // namespace keyboard_hook

} // namespace gbhk

#endif // GLOBAL_HOTKEY_WIN

#endif // !GLOBAL_HOTKEY_KEYBOARD_HOOK_HPP
