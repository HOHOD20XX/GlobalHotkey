#ifndef GLOBAL_HOTKEY_KEYBOARD_HOOK_HPP
#define GLOBAL_HOTKEY_KEYBOARD_HOOK_HPP

#include "core/core.hpp"

// Only usable in windows platform.
#ifdef _GLOBAL_HOTKEY_WIN

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
// If success return 0, else return error code.
uint run();
// If success return 0, else return error code.
uint end();

}

}

#endif // _GLOBAL_HOTKEY_WIN

#endif // !GLOBAL_HOTKEY_KEYBOARD_HOOK_HPP
