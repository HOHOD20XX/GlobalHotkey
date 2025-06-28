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

int _KBHMPrivateWin::doBeforeThreadEnd()
{
    if (PostThreadMessageA(workerThreadId, WM_DESTROY, WPARAM(), LPARAM()) != 0)
        return RC_SUCCESS;
    return GetLastError();
}

void _KBHMPrivateWin::work()
{
    HHOOK hhook = SetWindowsHookExA(WH_KEYBOARD_LL, &_KBHMPrivateWin::LowLevelKeyboardProc, NULL, 0);;
    if (!hhook)
        setRunFail(GetLastError());

    workerThreadId = GetCurrentThreadId();
    MSG msg = {0};
    // Force the system to create the message queue.
    PeekMessageA(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE);
    // Indicate the worker thread is created successfully after create the message queue.
    // This can ensure that the `PostThreadMessage` is called only when the message queue exists.
    setRunSuccess();
    // Retrieves only messages on the current thread's message queue whose hwnd value is NULL.
    // In this case the thread message as posted by `PostThreadMessage`.
    setRunSuccess();
    while (GetMessageA(&msg, HWND(-1), 0, 0) != 0)
    {
        if (msg.message == WM_DESTROY)
            PostQuitMessage(0);
    }

    UnhookWindowsHookEx(hhook);
}

LRESULT WINAPI _KBHMPrivateWin::LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode == HC_ACTION)
    {
        KBDLLHOOKSTRUCT* ptr = (KBDLLHOOKSTRUCT*) lParam;
        int nativeKey = ptr->vkCode;
        KeyState state = KS_NONE;

        if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)
        {
            auto keyPressedCallback = getKeyPressedCallback();
            if (keyPressedCallback)
                keyPressedCallback(nativeKey);
            state = KS_PRESSED;
        }
        else if (wParam == WM_KEYUP || wParam == WM_SYSKEYUP)
        {
            auto keyReleasedCallback = getKeyReleasedCallback();
            if (keyReleasedCallback)
                keyReleasedCallback(nativeKey);
            state = KS_RELEASED;
        }

        auto fn = getKeyListenerCallback(nativeKey, state);
        if (fn)
            fn();
    }

    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

} // namespace kbhook

} // namespace gbhk

#endif // _GLOBAL_HOTKEY_WIN

#endif // !GLOBAL_HOTKEY_DISABLE_HOOK
