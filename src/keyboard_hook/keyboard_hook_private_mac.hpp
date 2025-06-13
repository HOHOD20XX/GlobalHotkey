#ifndef GLOBAL_HOTKEY_KEYBOARD_HOOK_PRIVATE_MAC_HPP
#define GLOBAL_HOTKEY_KEYBOARD_HOOK_PRIVATE_MAC_HPP

#ifndef GLOBAL_HOTKEY_DISABLE_HOOK

#include "keyboard_hook_private.hpp"

#ifdef _GLOBAL_HOTKEY_MAC

namespace gbhk
{

namespace kbhook
{

class _KeyboardHookPrivateMac final : public _KeyboardHookPrivate
{
public:
    _KeyboardHookPrivateMac();
    ~_KeyboardHookPrivateMac();

protected:

private:
};

} // namespace kbhook

} // namespace gbhk

#endif // _GLOBAL_HOTKEY_MAC

#endif // !GLOBAL_HOTKEY_DISABLE_HOOK

#endif // !GLOBAL_HOTKEY_KEYBOARD_HOOK_PRIVATE_MAC_HPP
