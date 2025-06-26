#ifndef GLOBAL_HOTKEY_DISABLE_REGISTER

#include "register_ghm_private_win.hpp"

#ifdef _GLOBAL_HOTKEY_WIN

#include <global_hotkey/return_code.hpp>

#include "../key/key_private.hpp"

#define WM_REGISTER_HOTKEY (WM_USER + 1)
#define WM_UNREGISTER_HOTKEY (WM_USER + 2)

namespace gbhk
{

_RegisterGHMPrivateWin::_RegisterGHMPrivateWin() : workerThreadId(DWORD()), rc(RC_NOT_USED), hotkeyIndex(0) {}

_RegisterGHMPrivateWin::~_RegisterGHMPrivateWin() { end(); }

int _RegisterGHMPrivateWin::doBeforeThreadEnd()
{
    if (PostThreadMessageA(workerThreadId, WM_DESTROY, WPARAM(), LPARAM()) != 0)
        return RC_SUCCESS;
    return GetLastError();
}

void _RegisterGHMPrivateWin::work()
{
    workerThreadId = GetCurrentThreadId();
    MSG msg = {0};
    // Force the system to create the message queue.
    PeekMessageA(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE);
    // Indicate the worker thread is created successfully after create the message queue.
    // This can ensure that the `PostThreadMessage` is called only when the message queue exists.
    setSuccessRunning();
    // Retrieves only messages on the current thread's message queue whose hwnd value is NULL.
    // In this case the thread message as posted by `PostThreadMessage`.
    while (GetMessageA(&msg, HWND(-1), 0, 0) != 0)
    {
        if (msg.message == WM_HOTKEY)
        {
            invoke(msg.wParam, msg.lParam);
        }
        else if (msg.message == WM_REGISTER_HOTKEY)
        {
            rc = nativeRegisterHotkey(msg.wParam, msg.lParam);
            cvReturned.notify_one();
        }
        else if (msg.message == WM_UNREGISTER_HOTKEY)
        {
            rc = nativeUnregisterHotkey(msg.wParam, msg.lParam);
            cvReturned.notify_one();
        }
        else if (msg.message == WM_DESTROY)
        {
            PostQuitMessage(0);
        }
    }

    workerThreadId = DWORD();
    rc = RC_NOT_USED;
    hotkeyIndex = 0;
    hotkeyIdToKc.clear();
    kcToHotkeyId.clear();
}

int _RegisterGHMPrivateWin::registerHotkey(const KeyCombination& kc, bool autoRepeat)
{
    rc = RC_NOT_USED;
    if (PostThreadMessageA(workerThreadId, WM_REGISTER_HOTKEY, kc.combinedValue(), (LPARAM) autoRepeat) != 0)
    {
        std::mutex dummyLock;
        std::unique_lock<std::mutex> lock(dummyLock);
        cvReturned.wait(lock, [this]() { return (rc != RC_NOT_USED); });
        return rc;
    }
    return GetLastError();
}

int _RegisterGHMPrivateWin::unregisterHotkey(const KeyCombination& kc)
{
    rc = RC_NOT_USED;
    if (PostThreadMessageA(workerThreadId, WM_UNREGISTER_HOTKEY, kc.combinedValue(), LPARAM()) != 0)
    {
        std::mutex dummyLock;
        std::unique_lock<std::mutex> lock(dummyLock);
        cvReturned.wait(lock, [this]() { return (rc != RC_NOT_USED); });
        return rc;
    }
    return GetLastError();
}

void _RegisterGHMPrivateWin::invoke(WPARAM wParam, LPARAM lParam)
{
    int hotkeyId = int(wParam);
    if (hotkeyIdToKc.find(hotkeyId) != hotkeyIdToKc.end())
    {
        auto& kc = hotkeyIdToKc[hotkeyId];
        auto fn = getValue(kc).second;
        if (fn)
            fn();
    }
}

int _RegisterGHMPrivateWin::nativeRegisterHotkey(WPARAM wParam, LPARAM lParam)
{
    KeyCombination kc = KeyCombination::fromCombinedValue(wParam);
    bool autoRepeat = bool(lParam);
    int mod = nativeModifiers(kc.modifiers());
    mod |= (!autoRepeat ? MOD_NOREPEAT : 0);

    if (RegisterHotKey(NULL, hotkeyIndex, mod, nativeKey(kc.key())) != 0)
    {
        hotkeyIdToKc[hotkeyIndex] = kc;
        kcToHotkeyId[kc] = hotkeyIndex;
        hotkeyIndex++;
        return RC_SUCCESS;
    }
    return GetLastError();
}

int _RegisterGHMPrivateWin::nativeUnregisterHotkey(WPARAM wParam, LPARAM lParam)
{
    KeyCombination kc = KeyCombination::fromCombinedValue(wParam);
    int hotkeyId = kcToHotkeyId[kc];
    UnregisterHotKey(NULL, hotkeyId);
    hotkeyIdToKc.erase(hotkeyId);
    kcToHotkeyId.erase(kc);
    return RC_SUCCESS;
}

} // namespace gbhk

#endif // _GLOBAL_HOTKEY_WIN

#endif // !GLOBAL_HOTKEY_DISABLE_REGISTER
