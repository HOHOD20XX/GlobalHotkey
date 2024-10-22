#ifndef KEYBOARD_HOOK_HPP
#define KEYBOARD_HOOK_HPP

#include "core/core.hpp"

// Only usable in windows platform.
#ifdef GBHK_WIN

namespace gbhk
{

namespace keyboard_hook
{

enum State : uchar
{
    PRESSED,
    RELEASED
};

void addKeyEventCallback(uint key, State state, VoidFunc func);
void addKeyEventCallback(uint key, State state, ArgFunc func, Arg arg);
void setKeyPressedCallback(void (*func)(uint key));
void setKeyReleaseddCallback(void (*func)(uint key));
// If success return 0, else return error code.
uint run();
// If success return 0, else return error code.
uint end();

}

}

#endif // GBHK_WIN

#endif // !KEYBOARD_HOOK_HPP
