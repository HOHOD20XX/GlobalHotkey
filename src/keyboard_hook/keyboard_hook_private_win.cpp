#ifndef GLOBAL_HOTKEY_DISABLE_HOOK

#include "keyboard_hook_private_win.hpp"

#ifdef _GLOBAL_HOTKEY_WIN

#define KBHP _KeyboardHookPrivateWin

namespace gbhk
{

namespace kbhook
{

LRESULT WINAPI LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);

_KeyboardHookPrivateWin::_KeyboardHookPrivateWin() = default;

_KeyboardHookPrivateWin::~_KeyboardHookPrivateWin()
{
    end();
}

int _KeyboardHookPrivateWin::start()
{
    if (isRunning_)
        return RC_SUCCESS;

    hhook_ = ::SetWindowsHookExA(WH_KEYBOARD_LL, &LowLevelKeyboardProc, NULL, 0);
    if (hhook_)
    {
        isRunning_ = true;
        return RC_SUCCESS;
    }
    else
    {
        return ::GetLastError();
    }
}

int _KeyboardHookPrivateWin::end()
{
    if (!isRunning_)
        return RC_SUCCESS;

    int rtn = RC_SUCCESS;
    if (::UnhookWindowsHookEx(hhook_) == 0)
        rtn = ::GetLastError();

    _KeyboardHookPrivate::resetStaticMember_();
    hhook_ = nullptr;
    isRunning_ = false;

    return rtn;
}

LRESULT WINAPI LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode == HC_ACTION)
    {
        KBDLLHOOKSTRUCT* p = (KBDLLHOOKSTRUCT*) lParam;
        int nativeKey = p->vkCode;
        int state = 0;

        LOCK_MUTEX(_KeyboardHookPrivate::mtx_);

        if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)
        {
            if (KBHP::keyPressedCallback_)
                KBHP::keyPressedCallback_(nativeKey);
            state = KS_PRESSED;
        }
        else if (wParam == WM_KEYUP || wParam == WM_SYSKEYUP)
        {
            if (KBHP::keyReleasedCallback_)
                KBHP::keyReleasedCallback_(nativeKey);
            state = KS_RELEASED;
        }

        KBHP::Combine combine(nativeKey, static_cast<KeyState>(state));
        if (KBHP::funcs_.find(combine) != KBHP::funcs_.end())
        {
            auto& func = KBHP::funcs_[combine];
            if (func)
                func();
        }
    }

    return ::CallNextHookEx(NULL, nCode, wParam, lParam);
}

} // namespace kbhook

} // namespace gbhk

#endif // _GLOBAL_HOTKEY_WIN

#endif // !GLOBAL_HOTKEY_DISABLE_HOOK
