#ifndef GLOBAL_HOTKEY_DISABLE_HOOK

#include "hook_ghm_private.hpp"

#include <chrono>   // chrono

#include <global_hotkey/keyboard_hook.hpp>
#include <global_hotkey/utility.hpp>

#define NOWTP std::chrono::steady_clock::now()

namespace gbhk
{

std::atomic<Modifiers> _HookGHMPrivate::pressedModi_(0);
std::atomic<Key> _HookGHMPrivate::pressedKey_(0);

_HookGHMPrivate::_HookGHMPrivate() :
    debouncedTime_(_DEFAULT_DEBOUNCED_TIME)
{}

_HookGHMPrivate::~_HookGHMPrivate() = default;

int _HookGHMPrivate::start()
{
    if (isRunning_)
        return EC_SUCCESS;

    auto& keyboardHook = kbhook::KeyboardHook::getInstance();
    keyboardHook.setKeyPressedEvent(&pressedKeyCallback_);
    keyboardHook.setKeyReleasedEvent(&releasedKeyCallback_);
    int ec = keyboardHook.start();
    if (ec != EC_SUCCESS)
        return ec;

    isRunning_ = true;
    workThread_ = std::thread([=]()
    {
        workThreadId_ = CUR_TH_ID;

        TimedSleeper ts;
        KeyCombination prevKc;
        auto prevExecTime = NOWTP;
        while (!shouldClose_)
        {
            ts.resetStartTime();

            KeyCombination kc(pressedModi_, pressedKey_);

            bool isDebouncing = true;
            bool isInvalidDebouncedTime = debouncedTime_ == 0;
            bool isOnlyModiHasDiff =  kc.modifiers() != prevKc.modifiers() && kc.key() == prevKc.key();
            bool isPrevModiContainsCurModi = prevKc.modifiers().has(kc.modifiers());
            bool isInDebouncedTime = NOWTP - prevExecTime < std::chrono::milliseconds(debouncedTime_);
            if (isInvalidDebouncedTime || !isOnlyModiHasDiff || !isPrevModiContainsCurModi || !isInDebouncedTime)
                isDebouncing = false;

            if (!isDebouncing)
            {
                LOCK_MUTEX(mtx_);

                if (voidFuncs_.find(kc) != voidFuncs_.end())
                {
                    auto& pair = voidFuncs_[kc];
                    auto& autoRepeat = pair.first;
                    auto& func = pair.second;
                    bool canExec = (func != nullptr) && (kc != prevKc || autoRepeat);
                    if (canExec)
                        func();
                }
                else if (argFuncArgs_.find(kc) != argFuncArgs_.end())
                {
                    auto& pair = argFuncArgs_[kc];
                    auto& autoRepeat = pair.first;
                    auto& func = pair.second.first;
                    auto& arg = pair.second.second;
                    bool canExec = (func != nullptr) && (kc != prevKc || autoRepeat);
                    if (canExec)
                        func(arg);
                }

                prevKc = kc;
                prevExecTime = NOWTP;
            }

            ts.sleepUntilElapsed(cycleTime_);
        }

        isRunning_ = false;
    });
    workThread_.detach();
    return EC_SUCCESS;
}

int _HookGHMPrivate::end()
{
    if (!isRunning_)                    return EC_SUCCESS;
    if (CUR_TH_ID == workThreadId_)     return EC_END_GHM_IN_WRONG_THREAD;

    shouldClose_ = true;
    while (isRunning_)
        yield();
    shouldClose_ = false;
    pressedModi_ = 0;
    pressedKey_ = 0;
    workThreadId_ = std::thread::id();
    removeAll();

    return EC_SUCCESS;
}

int _HookGHMPrivate::add(const KeyCombination& kc, VoidFunc func, bool autoRepeat)
{
    if (!kc.isValid() || func == nullptr)   return EC_INVALID_VALUE;
    if (has(kc))                            return EC_EXIST_SAME_VALUE;

    LOCK_MUTEX(mtx_);
    voidFuncs_.insert({ kc, { autoRepeat, func } });
    return EC_SUCCESS;
}

int _HookGHMPrivate::add(const KeyCombination& kc, ArgFunc func, Arg arg, bool autoRepeat)
{
    if (!kc.isValid() || func == nullptr)   return EC_INVALID_VALUE;
    if (has(kc))                            return EC_EXIST_SAME_VALUE;

    LOCK_MUTEX(mtx_);
    argFuncArgs_.insert({ kc, { autoRepeat, { func, arg } } });
    return EC_SUCCESS;
}

int _HookGHMPrivate::remove(const KeyCombination& kc)
{
    LOCK_MUTEX(mtx_);

    if (voidFuncs_.find(kc) != voidFuncs_.end())
        voidFuncs_.erase(kc);
    else if (argFuncArgs_.find(kc) != argFuncArgs_.end())
        argFuncArgs_.erase(kc);
    else
        return EC_NO_SPECIFIED_VALUE;

    return EC_SUCCESS;
}

int _HookGHMPrivate::removeAll()
{
    LOCK_MUTEX(mtx_);
    voidFuncs_.clear();
    argFuncArgs_.clear();
    return EC_SUCCESS;
}

int _HookGHMPrivate::replace(const KeyCombination& oldKc, const KeyCombination& newKc)
{
    if (!newKc.isValid())   return EC_INVALID_VALUE;
    if (oldKc == newKc)     return EC_NO_CHANGED_VALUE;

    LOCK_MUTEX(mtx_);

    if (voidFuncs_.find(oldKc) != voidFuncs_.end())
    {
        auto& pair = voidFuncs_[oldKc];
        voidFuncs_.erase(oldKc);
        voidFuncs_.insert({ newKc, pair });
    }
    else if (argFuncArgs_.find(oldKc) != argFuncArgs_.end())
    {
        auto& pair = argFuncArgs_[oldKc];
        argFuncArgs_.erase(oldKc);
        argFuncArgs_.insert({ newKc, pair });
    }
    else
    {
        return EC_NO_SPECIFIED_VALUE;
    }

    return EC_SUCCESS;
}

int _HookGHMPrivate::setAutoRepeat(const KeyCombination& kc, bool autoRepeat)
{
    LOCK_MUTEX(mtx_);

    if (voidFuncs_.find(kc) != voidFuncs_.end())
    {
        auto& pair = voidFuncs_[kc];
        if (pair.first == autoRepeat)
            return EC_NO_CHANGED_VALUE;
        pair.first = autoRepeat;
    }
    else if (argFuncArgs_.find(kc) != argFuncArgs_.end())
    {
        auto& pair = argFuncArgs_[kc];
        if (pair.first == autoRepeat)
            return EC_NO_CHANGED_VALUE;
        pair.first = autoRepeat;
    }
    else
    {
        return EC_NO_SPECIFIED_VALUE;
    }

    return EC_SUCCESS;
}

int _HookGHMPrivate::setDebouncedTime(size_t milliseconds)
{
    debouncedTime_ = milliseconds;
    return EC_SUCCESS;
}

void _HookGHMPrivate::pressedKeyCallback_(int nativeKey)
{
    auto key = getKeyFromNative(nativeKey);
    if (key == KEY_MODI_META || key == KEY_MODI_META_LEFT || key == KEY_MODI_META_RIGHT)
        pressedModi_ = pressedModi_.load().add(MODI_META);
    else if (key == KEY_MODI_CTRL || key == KEY_MODI_CTRL_LEFT || key == KEY_MODI_CTRL_RIGHT)
        pressedModi_ = pressedModi_.load().add(MODI_CTRL);
    else if (key == KEY_MODI_ALT || key == KEY_MODI_ALT_LEFT || key == KEY_MODI_ALT_RIGHT)
        pressedModi_ = pressedModi_.load().add(MODI_ALT);
    else if (key == KEY_MODI_SHIFT || key == KEY_MODI_SHIFT_LEFT || key == KEY_MODI_SHIFT_RIGHT)
        pressedModi_ = pressedModi_.load().add(MODI_SHIFT);
    else
        pressedKey_ = key;
}

void _HookGHMPrivate::releasedKeyCallback_(int nativeKey)
{
    auto key = getKeyFromNative(nativeKey);
    if (key == KEY_MODI_META || key == KEY_MODI_META_LEFT || key == KEY_MODI_META_RIGHT)
        pressedModi_ = pressedModi_.load().remove(MODI_META);
    else if (key == KEY_MODI_CTRL || key == KEY_MODI_CTRL_LEFT || key == KEY_MODI_CTRL_RIGHT)
        pressedModi_ = pressedModi_.load().remove(MODI_CTRL);
    else if (key == KEY_MODI_ALT || key == KEY_MODI_ALT_LEFT || key == KEY_MODI_ALT_RIGHT)
        pressedModi_ = pressedModi_.load().remove(MODI_ALT);
    else if (key == KEY_MODI_SHIFT || key == KEY_MODI_SHIFT_LEFT || key == KEY_MODI_SHIFT_RIGHT)
        pressedModi_ = pressedModi_.load().remove(MODI_SHIFT);
    else
        pressedKey_ = 0;
}

} // namespace gbhk

#endif // !GLOBAL_HOTKEY_DISABLE_HOOK
