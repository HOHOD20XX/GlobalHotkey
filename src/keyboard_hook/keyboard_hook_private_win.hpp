#ifndef GLOBAL_HOTKEY_KEYBOARD_HOOK_PRIVATE_WIN_HPP
#define GLOBAL_HOTKEY_KEYBOARD_HOOK_PRIVATE_WIN_HPP

#ifndef GLOBAL_HOTKEY_DISABLE_HOOK

#include "keyboard_hook_private.hpp"

#ifdef _GLOBAL_HOTKEY_WIN

#include <windows.h>

namespace gbhk
{

namespace kbhook
{

class _KeyboardHookPrivateWin final : public _KeyboardHookPrivate
{
public:
    _KeyboardHookPrivateWin();
    ~_KeyboardHookPrivateWin();

    int start() override;
    int end() override;

private:
    friend LRESULT LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);

    HHOOK hhook_ = nullptr;
};

} // namespace kbhook

} // namespace gbhk

#endif // _GLOBAL_HOTKEY_WIN

#endif // !GLOBAL_HOTKEY_DISABLE_HOOK

#endif // !GLOBAL_HOTKEY_KEYBOARD_HOOK_PRIVATE_WIN_HPP
