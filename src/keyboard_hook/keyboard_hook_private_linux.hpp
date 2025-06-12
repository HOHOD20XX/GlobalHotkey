#ifndef GLOBAL_HOTKEY_KEYBOARD_HOOK_PRIVATE_LINUX_HPP
#define GLOBAL_HOTKEY_KEYBOARD_HOOK_PRIVATE_LINUX_HPP

#ifndef GLOBAL_HOTKEY_DISABLE_HOOK

#include "keyboard_hook_private.hpp"

#ifdef _GLOBAL_HOTKEY_LINUX

namespace gbhk
{

namespace kbhook
{

class _KeyboardHookPrivateLinux final : public _KeyboardHookPrivate
{
public:
    _KeyboardHookPrivateLinux() noexcept = default;
    ~_KeyboardHookPrivateLinux() noexcept = default;

protected:

private:
};

} // namespace kbhook

} // namespace gbhk

#endif // _GLOBAL_HOTKEY_LINUX

#endif // !GLOBAL_HOTKEY_DISABLE_HOOK

#endif // !GLOBAL_HOTKEY_KEYBOARD_HOOK_PRIVATE_LINUX_HPP
