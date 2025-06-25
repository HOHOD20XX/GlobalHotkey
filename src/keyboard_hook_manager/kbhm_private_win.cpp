#ifndef GLOBAL_HOTKEY_DISABLE_HOOK

#include "kbhm_private_win.hpp"

#ifdef _GLOBAL_HOTKEY_WIN

#include <global_hotkey/return_code.hpp>

namespace gbhk
{

namespace kbhook
{

_KBHMPrivateWin::_KBHMPrivateWin() = default;

_KBHMPrivateWin::~_KBHMPrivateWin() { end(); }

int _KBHMPrivateWin::doBeforeLoop()
{
    hhook = SetWindowsHookExA(WH_KEYBOARD_LL, &_KBHMPrivateWin::LowLevelKeyboardProc, NULL, 0);;
    if (hhook)
        return RC_SUCCESS;
    else
        return GetLastError();
}

int _KBHMPrivateWin::doAfterLoop()
{
    UnhookWindowsHookEx(hhook);
    msg = {0};
    hhook = NULL;
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

LRESULT WINAPI _KBHMPrivateWin::LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode == HC_ACTION)
    {
        KBDLLHOOKSTRUCT* ptr = (KBDLLHOOKSTRUCT*) lParam;
        int nativeKey = ptr->vkCode;
        int state = 0;

        std::lock_guard<std::mutex> lock(mtx);

        if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)
        {
            if (keyPressedCallback)
                keyPressedCallback(nativeKey);
            state = KS_PRESSED;
        }
        else if (wParam == WM_KEYUP || wParam == WM_SYSKEYUP)
        {
            if (keyReleasedCallback)
                keyReleasedCallback(nativeKey);
            state = KS_RELEASED;
        }

        Combine combine(nativeKey, static_cast<KeyState>(state));
        if (fns.find(combine) != fns.end())
        {
            auto& fn = fns[combine];
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
