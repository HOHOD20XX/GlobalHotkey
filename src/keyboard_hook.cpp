#include "global_hotkey/keyboard_hook.hpp"

#if defined(GLOBAL_HOTKEY_WIN) && !defined(GLOBAL_HOTKEY_NOHOOK)

#include <atomic>           // atomic
#include <mutex>            // mutex, lock_guard
#include <unordered_map>    // unordered_map

#include <Windows.h>

#include "global_hotkey/core/return_code.hpp"

namespace gbhk
{

namespace keyboard_hook
{

static std::mutex gMtx;
static HHOOK gHhook = nullptr;
static void (*gKeyPressedCallback)(int key) = nullptr;
static void (*gKeyReleasedCallback)(int key) = nullptr;
static std::unordered_map<int, std::pair<State, VoidFunc>> gVoidFuncs;
static std::unordered_map<int, std::pair<State, ArgFuncArg>> gArgFuncArgs;

static LRESULT WINAPI LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode == HC_ACTION)
    {
        KBDLLHOOKSTRUCT* p = (KBDLLHOOKSTRUCT*) lParam;
        auto key = p->vkCode;

        std::lock_guard<std::mutex> lock(gMtx);

        if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)
        {
            if (gKeyPressedCallback)
                gKeyPressedCallback(key);
        }
        else if (wParam == WM_KEYUP || wParam == WM_SYSKEYUP)
        {
            if (gKeyReleasedCallback)
                gKeyReleasedCallback(key);
        }

        if (gVoidFuncs.find(key) != gVoidFuncs.end())
        {
            bool keydownExe = gVoidFuncs[key].first == PRESSED && (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN);
            bool keyupExe = gVoidFuncs[key].first == RELEASED && (wParam == WM_KEYUP || wParam == WM_SYSKEYUP);

            if (keydownExe || keyupExe)
                gVoidFuncs[key].second();
        }

        if (gArgFuncArgs.find(key) != gArgFuncArgs.end())
        {
            bool keydownExe = gVoidFuncs[key].first == PRESSED && (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN);
            bool keyupExe = gVoidFuncs[key].first == RELEASED && (wParam == WM_KEYUP || wParam == WM_SYSKEYUP);

            if (keydownExe || keyupExe)
            {
                auto& fn = gArgFuncArgs[key].second.first;
                auto& arg = gArgFuncArgs[key].second.second;
                fn(arg);
            }
        }
    }

    return ::CallNextHookEx(NULL, nCode, wParam, lParam);
}

void addKeyEventCallback(int key, State state, VoidFunc callbackFunc)
{
    std::lock_guard<std::mutex> lock(gMtx);
    if (gArgFuncArgs.find(key) != gArgFuncArgs.end() && gArgFuncArgs[key].first == state)
        gArgFuncArgs.erase(key);
    gVoidFuncs.insert({ key, { state, callbackFunc } });
}

void addKeyEventCallback(int key, State state, ArgFunc callbackFunc, Arg arg)
{
    std::lock_guard<std::mutex> lock(gMtx);
    if (gVoidFuncs.find(key) != gVoidFuncs.end() && gVoidFuncs[key].first == state)
        gVoidFuncs.erase(key);
    gArgFuncArgs.insert({ key, { state, { callbackFunc, arg } } });
}

void setKeyPressedCallback(void (*callbackFunc)(int key))
{
    std::lock_guard<std::mutex> lock(gMtx);
    gKeyPressedCallback = callbackFunc;
}

void setKeyReleaseddCallback(void (*callbackFunc)(int key))
{
    std::lock_guard<std::mutex> lock(gMtx);
    gKeyReleasedCallback = callbackFunc;
}

int start()
{
    gHhook = ::SetWindowsHookExA(WH_KEYBOARD_LL, &LowLevelKeyboardProc, NULL, 0);
    if (gHhook)
        return RC_SUCCESS;
    else
        return ::GetLastError();
}

int end()
{
    if (::UnhookWindowsHookEx(gHhook) != 0)
    {
        gHhook = nullptr;
        gKeyPressedCallback = nullptr;
        gKeyReleasedCallback = nullptr;
        gVoidFuncs.clear();
        gArgFuncArgs.clear();

        return RC_SUCCESS;
    }
    else
    {
        return ::GetLastError();
    }
}

}

} // namespace gbhk

#endif // GLOBAL_HOTKEY_WIN && !GLOBAL_HOTKEY_NOHOOK
