#ifndef GLOBAL_HOTKEY_DISABLE_HOOK

#include "keyboard_hook_private_mac.hpp"

#ifdef _GLOBAL_HOTKEY_MAC

namespace gbhk
{

namespace kbhook
{

_KeyboardHookPrivateMac::_KeyboardHookPrivateMac() = default;

_KeyboardHookPrivateMac::~_KeyboardHookPrivateMac()
{
    end();
}

int _KeyboardHookPrivateMac::start()
{

}

int _KeyboardHookPrivateMac::end()
{

}

} // namespace kbhook

} // namespace gbhk

#endif // _GLOBAL_HOTKEY_MAC

#endif // !GLOBAL_HOTKEY_DISABLE_HOOK
