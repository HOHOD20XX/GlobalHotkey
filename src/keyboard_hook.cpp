#include <global_hotkey/keyboard_hook.hpp>

// Only usable in windows platform.
#ifdef GBHK_WIN

#include <atomic>
#include <mutex>
#include <unordered_map>

#include <Windows.h>

namespace gbhk
{

namespace keyboard_hook
{

#ifdef GBHK_CPP17
inline std::mutex kMtx;
inline HHOOK kHhook = nullptr;
inline void (*kKeyPressedCallback)(uint key) = nullptr;
inline void (*kKeyReleasedCallback)(uint key) = nullptr;
inline std::unordered_map<uint, std::pair<State, VoidFunc>> kVoidFuncs;
inline std::unordered_map<uint, std::pair<State, ArgFuncArg>> kArgFuncArgs;
#else
static std::mutex kMtx;
static HHOOK kHhook = nullptr;
static void (*kKeyPressedCallback)(uint key) = nullptr;
static void (*kKeyReleasedCallback)(uint key) = nullptr;
static std::unordered_map<uint, std::pair<State, VoidFunc>> kVoidFuncs;
static std::unordered_map<uint, std::pair<State, ArgFuncArg>> kArgFuncArgs;
#endif // GBHK_CPP17

static LRESULT WINAPI LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode == HC_ACTION) {
        KBDLLHOOKSTRUCT* p = (KBDLLHOOKSTRUCT*) lParam;
        auto key = p->vkCode;

        kMtx.lock();

        if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {
            if (kKeyPressedCallback)
                kKeyPressedCallback(key);
        } else if (wParam == WM_KEYUP || wParam == WM_SYSKEYUP) {
            if (kKeyReleasedCallback)
                kKeyReleasedCallback(key);
        }

        if (kVoidFuncs.find(key) != kVoidFuncs.end()) {
            bool keydownExe =
                kVoidFuncs[key].first == PRESSED &&
                (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN);
            bool keyupExe =
                kVoidFuncs[key].first == RELEASED &&
                (wParam == WM_KEYUP || wParam == WM_SYSKEYUP);
            if (keydownExe || keyupExe)
                kVoidFuncs[key].second();
        }

        if (kArgFuncArgs.find(key) != kArgFuncArgs.end()) {
            bool keydownExe =
                kVoidFuncs[key].first == PRESSED &&
                (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN);
            bool keyupExe =
                kVoidFuncs[key].first == RELEASED &&
                (wParam == WM_KEYUP || wParam == WM_SYSKEYUP);
            if (keydownExe || keyupExe) {
                auto& fn = kArgFuncArgs[key].second.first;
                auto& arg = kArgFuncArgs[key].second.second;
                fn(arg);
            }
        }

        kMtx.unlock();
    }

    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

void addKeyEventCallback(uint key, State state, VoidFunc callbackFunc)
{
    kMtx.lock();

    if (kArgFuncArgs.find(key) != kArgFuncArgs.end() && kArgFuncArgs[key].first == state) {
        kArgFuncArgs.erase(key);
    }
    kVoidFuncs.insert({ key, { state, callbackFunc } });

    kMtx.unlock();
}

void addKeyEventCallback(uint key, State state, ArgFunc callbackFunc, Arg arg)
{
    kMtx.lock();

    if (kVoidFuncs.find(key) != kVoidFuncs.end() && kVoidFuncs[key].first == state) {
        kVoidFuncs.erase(key);
    }
    kArgFuncArgs.insert({ key, { state, { callbackFunc, arg } } });

    kMtx.unlock();
}

void setKeyPressedCallback(void (*callbackFunc)(uint key))
{
    kMtx.lock();
    kKeyPressedCallback = callbackFunc;
    kMtx.unlock();
}

void setKeyReleaseddCallback(void (*callbackFunc)(uint key))
{
    kMtx.lock();
    kKeyReleasedCallback = callbackFunc;
    kMtx.unlock();
}

uint run()
{
    kHhook = ::SetWindowsHookExA(WH_KEYBOARD_LL, LowLevelKeyboardProc, NULL, 0);

    if (kHhook)
        return GBHK_RSLT_SUCCESS;
    return ::GetLastError();
}

uint end()
{
    if (::UnhookWindowsHookEx(kHhook)) {
        kHhook = nullptr;
        kKeyPressedCallback = nullptr;
        kKeyReleasedCallback = nullptr;
        kVoidFuncs.clear();
        kArgFuncArgs.clear();
        return GBHK_RSLT_SUCCESS;
    }
    return ::GetLastError();
}

}

}

#endif // GBHK_WIN
