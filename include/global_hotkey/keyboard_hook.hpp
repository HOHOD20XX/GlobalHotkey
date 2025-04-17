#ifndef GLOBAL_HOTKEY_KEYBOARD_HOOK_HPP
#define GLOBAL_HOTKEY_KEYBOARD_HOOK_HPP

#include "core/base.hpp"

#if defined(GLOBAL_HOTKEY_WIN) && !defined(GLOBAL_HOTKEY_NOHOOK)

namespace gbhk
{

namespace keyboard_hook
{

enum State
{
    PRESSED,
    RELEASED
};

void GBHK_API addKeyEventCallback(int key, State state, VoidFunc callbackFunc);

void GBHK_API addKeyEventCallback(int key, State state, ArgFunc callbackFunc, Arg arg);

void GBHK_API setKeyPressedCallback(void (*callbackFunc)(int key));

void GBHK_API setKeyReleaseddCallback(void (*callbackFunc)(int key));

/// @return If success return #RC_SUCCESS, else return error code.
int GBHK_API run();

/// @return If success return #RC_SUCCESS, else return error code.
int GBHK_API end();

} // namespace keyboard_hook

} // namespace gbhk

#endif // GLOBAL_HOTKEY_WIN && !GLOBAL_HOTKEY_NOHOOK

#endif // !GLOBAL_HOTKEY_KEYBOARD_HOOK_HPP
