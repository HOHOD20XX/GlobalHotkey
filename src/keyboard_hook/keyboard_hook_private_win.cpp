#ifndef GLOBAL_HOTKEY_DISABLE_HOOK

#include "keyboard_hook_private_win.hpp"

#ifdef _GLOBAL_HOTKEY_WIN

#define KBHP _KeyboardHookPrivateWin

namespace gbhk
{

namespace kbhook
{

LRESULT WINAPI LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);

_KeyboardHookPrivateWin::_KeyboardHookPrivateWin() :
    isThreadFinished_(false), shouldClose_(false)
{}

_KeyboardHookPrivateWin::~_KeyboardHookPrivateWin()
{
    end();
}

int _KeyboardHookPrivateWin::start()
{
    if (isRunning_)
        return RC_SUCCESS;

    int rtn = RC_SUCCESS;
    isRunning_ = true;
    workThread_ = std::thread([this, &rtn]() {
        hhook_ = ::SetWindowsHookExA(WH_KEYBOARD_LL, &LowLevelKeyboardProc, NULL, 0);
        if (hhook_)
        {
            isThreadFinished_ = true;
            cvIsThreadFinished_.notify_all();

            messageLoop_();
            isRunning_ = false;
            cvIsRunning_.notify_all();
        }
        else
        {
            isThreadFinished_ = true;
            cvIsThreadFinished_.notify_all();

            rtn = ::GetLastError();
            isRunning_ = false;
        }
    });
    workThread_.detach();

    std::unique_lock<std::mutex> lock(mtx_);
    cvIsThreadFinished_.wait(lock, [this]() { return isThreadFinished_.load(); });
    isThreadFinished_ = false;

    return rtn;
}

int _KeyboardHookPrivateWin::end()
{
    if (!isRunning_)
        return RC_SUCCESS;

    int rtn = RC_SUCCESS;
    if (::UnhookWindowsHookEx(hhook_) == 0)
        rtn = ::GetLastError();

    shouldClose_ = true;
    std::unique_lock<std::mutex> lock(mtx_);
    cvIsRunning_.wait(lock, [this]() {return !isRunning_; });
    lock.unlock();
    shouldClose_ = false;

    _KeyboardHookPrivate::resetStaticMember_();
    hhook_ = nullptr;

    return rtn;
}

void _KeyboardHookPrivateWin::messageLoop_()
{
    MSG msg = {0};
    while (!shouldClose_)
    {
        while (::PeekMessageA(&msg, NULL, WM_KEYFIRST, WM_KEYLAST, PM_REMOVE) != 0)
        {
            ::TranslateMessage(&msg);
            ::DispatchMessageA(&msg);
        }
    }
}

LRESULT WINAPI LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode == HC_ACTION)
    {
        KBDLLHOOKSTRUCT* p = (KBDLLHOOKSTRUCT*) lParam;
        int nativeKey = p->vkCode;
        int state = 0;

        std::lock_guard<std::mutex> lock(KBHP::mtx_);

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
        if (KBHP::fns_.find(combine) != KBHP::fns_.end())
        {
            auto& fn = KBHP::fns_[combine];
            if (fn)
                fn();
        }
    }

    return ::CallNextHookEx(NULL, nCode, wParam, lParam);
}

} // namespace kbhook

} // namespace gbhk

#endif // _GLOBAL_HOTKEY_WIN

#endif // !GLOBAL_HOTKEY_DISABLE_HOOK
