#ifndef GLOBAL_HOTKEY_DISABLE_HOOK

#include "kbhm_private_win.hpp"

#ifdef _GLOBAL_HOTKEY_WIN

#include <global_hotkey/return_code.hpp>

#define KBHMP _KBHMPrivateWin

namespace gbhk
{

namespace kbhook
{

LRESULT WINAPI LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);

_KBHMPrivateWin::_KBHMPrivateWin() = default;

_KBHMPrivateWin::~_KBHMPrivateWin() { end(); }

int _KBHMPrivateWin::specializedEnd()
{
    msg = {0};
    hhook = nullptr;
    return RC_SUCCESS;
}

int _KBHMPrivateWin::doBeforeLoop()
{
    hhook = SetWindowsHookExA(WH_KEYBOARD_LL, &LowLevelKeyboardProc, NULL, 0);;
    if (hhook)
        return RC_SUCCESS;
    else
        return GetLastError();
}

int _KBHMPrivateWin::doAfterLoop()
{
    UnhookWindowsHookEx(hhook);
    return RC_SUCCESS;
}

void _KBHMPrivateWin::eachCycleDo()
{
    while (PeekMessageA(&msg, NULL, WM_KEYFIRST, WM_KEYLAST, PM_REMOVE) != 0)
    {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }
}

LRESULT WINAPI LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode == HC_ACTION)
    {
        KBDLLHOOKSTRUCT* ptr = (KBDLLHOOKSTRUCT*) lParam;
        int nativeKey = ptr->vkCode;
        int state = 0;

        std::lock_guard<std::mutex> lock(KBHMP::mtx);

        if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)
        {
            if (KBHMP::keyPressedCallback)
                KBHMP::keyPressedCallback(nativeKey);
            state = KS_PRESSED;
        }
        else if (wParam == WM_KEYUP || wParam == WM_SYSKEYUP)
        {
            if (KBHMP::keyReleasedCallback)
                KBHMP::keyReleasedCallback(nativeKey);
            state = KS_RELEASED;
        }

        KBHMP::Combine combine(nativeKey, static_cast<KeyState>(state));
        if (KBHMP::fns.find(combine) != KBHMP::fns.end())
        {
            auto& fn = KBHMP::fns[combine];
            if (fn)
                fn();
        }
    }

    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

} // namespace kbhook

} // namespace gbhk

#endif // _GLOBAL_HOTKEY_WIN

#endif // !GLOBAL_HOTKEY_DISABLE_HOOK
