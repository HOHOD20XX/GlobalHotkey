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
        auto key = p->vkCode;

        LOCK_MUTEX(_KeyboardHookPrivate::mtx_);

        if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)
        {
            if (KBHP::keyPressedCallback_)
                KBHP::keyPressedCallback_(key);
        }
        else if (wParam == WM_KEYUP || wParam == WM_SYSKEYUP)
        {
            if (KBHP::keyReleasedCallback_)
                KBHP::keyReleasedCallback_(key);
        }

        if (KBHP::voidFuncs_.find(key) != KBHP::voidFuncs_.end())
        {
            bool keydownExec = (KBHP::voidFuncs_[key].first == KS_PRESSED) &&
                               (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN);
            bool keyupExec = (KBHP::voidFuncs_[key].first == KS_RELEASED) &&
                             (wParam == WM_KEYUP || wParam == WM_SYSKEYUP);

            auto& func = KBHP::voidFuncs_[key].second;
            if ((keydownExec || keyupExec) && func)
                func();
        }
        else if (KBHP::argFuncArgs_.find(key) != KBHP::argFuncArgs_.end())
        {
            bool keydownExec = (KBHP::argFuncArgs_[key].first == KS_PRESSED) &&
                               (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN);
            bool keyupExec = (KBHP::argFuncArgs_[key].first == KS_RELEASED) &&
                             (wParam == WM_KEYUP || wParam == WM_SYSKEYUP);

            auto& func = KBHP::argFuncArgs_[key].second.first;
            auto& arg = KBHP::argFuncArgs_[key].second.second;
            if ((keydownExec || keyupExec) && func)
                func(arg);
        }
    }

    return ::CallNextHookEx(NULL, nCode, wParam, lParam);
}

} // namespace kbhook

} // namespace gbhk

#endif // _GLOBAL_HOTKEY_WIN

#endif // !GLOBAL_HOTKEY_DISABLE_HOOK
