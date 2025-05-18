#include "global_hotkey/hook_global_hotkey.hpp"

#if defined(GLOBAL_HOTKEY_WIN) && !defined(GLOBAL_HOTKEY_NOHOOK)

#include <Windows.h>

#include "global_hotkey/keyboard_hook.hpp"

namespace gbhk
{

std::mutex HookGlobalHotkey::mtxListenKeyChanged_;
KeyCombination HookGlobalHotkey::pressed_;

HookGlobalHotkey::HookGlobalHotkey() :
    debouncedTime_(0)
{}

HookGlobalHotkey::~HookGlobalHotkey()
{
    int rslt = end();
}

HookGlobalHotkey& HookGlobalHotkey::getInstance()
{
    static HookGlobalHotkey instance;
    return instance;
}

int HookGlobalHotkey::start()
{
    namespace chr = std::chrono;

    // If is running do nothing.
    if (isRunning_)
        return RC_UNTIMELY_CALL;

    // Set the callback function of key pressed and key released.
    keyboard_hook::setKeyPressedCallback(&addPressedKey_);
    keyboard_hook::setKeyReleaseddCallback(&removePressedKey_);
    // Start hook the #LowKeyboardEvent.
    int rslt = keyboard_hook::run();
    // If failed to hook return error code.
    if (rslt != RC_SUCCESS)
        return rslt;

    isRunning_ = true;
    workThread_ = std::thread([&]()
    {
        // Get the current thread id and set the #workThreadId_ value.
        setWorkThreadId_(std::this_thread::get_id());

        // Record the previous pressed key combination.
        KeyCombination prevKeycomb;
        chr::steady_clock::time_point prevWorkTime = chr::steady_clock::now();
        while (!shouldClose_)
        {
            setTimePoint_();

            // Get the current pressed key combination.
            mtxListenKeyChanged_.lock();
            KeyCombination keycomb = pressed_;
            mtxListenKeyChanged_.unlock();

            bool isPass = false;
            if (debouncedTime_ == 0)
            {
                isPass = true;
            }
            else
            {
                bool isJustModDiff =
                     keycomb.key() == prevKeycomb.key() && keycomb.modifiers() != prevKeycomb.modifiers();
                bool isPrevModContainsCurMod =
                     prevKeycomb.modifiers() > keycomb.modifiers() &&
                     ((prevKeycomb.modifiers() & keycomb.modifiers()) == keycomb.modifiers());
                bool isInDebouncedTime =
                     (chr::steady_clock::now() - prevWorkTime) <= chr::milliseconds(debouncedTime_);
                if (!isJustModDiff || !isPrevModContainsCurMod || !isInDebouncedTime)
                    isPass = true;
            }

            if (isPass)
            {
                mtxFuncsOperate_.lock();

                if (voidFuncs_.find(keycomb) != voidFuncs_.end())
                {
                    // If the current keycomb not equal to previous keycomb or
                    // hotkey is auto repeat execute related function.
                    if ((prevKeycomb != keycomb) || voidFuncs_[keycomb].first)
                        voidFuncs_[keycomb].second();
                }
                else if (argFuncArgs_.find(keycomb) != argFuncArgs_.end())
                {
                    auto& fnArg = argFuncArgs_[keycomb].second;
                    // Ditto.
                    if ((prevKeycomb != keycomb) || argFuncArgs_[keycomb].first)
                        fnArg.first(fnArg.second);
                }

                mtxFuncsOperate_.unlock();

                prevKeycomb = keycomb;

                prevWorkTime = chr::steady_clock::now();
            }

            waitInterval_();
        }

        isRunning_ = false;
    });
    workThread_.detach();

    return RC_SUCCESS;
}

int HookGlobalHotkey::end()
{
    // If is not running do nothing.
    if (!isRunning_)
        return RC_UNTIMELY_CALL;

    // This function can't called in the work thread, else program get stuck in a loop.
    if (std::this_thread::get_id() == getWorkThreadId_())
        return RC_CALL_IN_WRONG_THREAD;

    // Set this flag to true to exit thread.
    shouldClose_ = true;
    // Wait the thread exits.
    while (isRunning_)
        sleep(1);
    // Reset this flag to default state.
    shouldClose_ = false;

    // Clear members.
    // Because the thread is exited, so not has thread safe problem.
    voidFuncs_.clear();
    argFuncArgs_.clear();

    return keyboard_hook::end();
}

int HookGlobalHotkey::add(const KeyCombination& keycomb, VoidFunc callbackFunc)
{
    if (!keycomb.isValid())
        return RC_INVALID_KEY_COMBINATION;

    int rslt = RC_SUCCESS;

    KeyCombination _keycomb(keycomb.modifiers(), keycomb.nativeKey(), keycomb.isAutoRepeat());

    mtxFuncsOperate_.lock();

    if (voidFuncs_.find(_keycomb) != voidFuncs_.end())
        rslt = RC_ALREADY_EXISTED;
    else
        voidFuncs_.insert({ _keycomb, { _keycomb.isAutoRepeat(), callbackFunc } });

    mtxFuncsOperate_.unlock();

    return rslt;
}

int HookGlobalHotkey::add(const KeyCombination& keycomb, ArgFunc callbackFunc, Arg arg)
{
    if (!keycomb.isValid())
        return RC_INVALID_KEY_COMBINATION;

    int rslt = RC_SUCCESS;

    KeyCombination _keycomb(keycomb.modifiers(), keycomb.nativeKey(), keycomb.isAutoRepeat());

    mtxFuncsOperate_.lock();

    if (argFuncArgs_.find(_keycomb) != argFuncArgs_.end())
        rslt = RC_ALREADY_EXISTED;
    else
        argFuncArgs_.insert({ _keycomb, { _keycomb.isAutoRepeat(), { callbackFunc, arg } } });

    mtxFuncsOperate_.unlock();

    return rslt;
}

int HookGlobalHotkey::remove(const KeyCombination& keycomb)
{
    int rslt = RC_SUCCESS;

    KeyCombination _keycomb(keycomb.modifiers(), keycomb.nativeKey(), keycomb.isAutoRepeat());

    mtxFuncsOperate_.lock();

    if (voidFuncs_.find(_keycomb) == voidFuncs_.end() && argFuncArgs_.find(_keycomb) == argFuncArgs_.end())
    {
        rslt = RC_NOT_FIND;
    }
    else
    {
        voidFuncs_.erase(_keycomb);
        argFuncArgs_.erase(_keycomb);
    }

    mtxFuncsOperate_.unlock();

    return rslt;
}

int HookGlobalHotkey::replace(const KeyCombination& oldKeycomb, const KeyCombination& newKeycomb)
{
    if (!newKeycomb.isValid())
        return RC_INVALID_KEY_COMBINATION;

    // If the old key combination equal to new key combination do nothing.
    if (oldKeycomb == newKeycomb && oldKeycomb.isAutoRepeat() == newKeycomb.isAutoRepeat())
        return RC_OLD_EQUAL_NEW;

    int rslt = RC_SUCCESS;

    KeyCombination _oldKeycomb(oldKeycomb.modifiers(), oldKeycomb.nativeKey(), oldKeycomb.isAutoRepeat());
    KeyCombination _newKeycomb(newKeycomb.modifiers(), newKeycomb.nativeKey(), newKeycomb.isAutoRepeat());

    mtxFuncsOperate_.lock();

    if (voidFuncs_.find(_oldKeycomb) != voidFuncs_.end())
    {
        auto func = voidFuncs_[_oldKeycomb].second;

        voidFuncs_.erase(_oldKeycomb);
        voidFuncs_.insert({ _newKeycomb, { _newKeycomb.isAutoRepeat(), func } });
    }
    else if (argFuncArgs_.find(_oldKeycomb) != argFuncArgs_.end())
    {
        auto funcArg = argFuncArgs_[_oldKeycomb].second;

        argFuncArgs_.erase(_oldKeycomb);
        argFuncArgs_.insert({ _newKeycomb, { _newKeycomb.isAutoRepeat(), funcArg } });
    }
    else
    {
        rslt = RC_NOT_FIND;
    }

    mtxFuncsOperate_.unlock();

    return rslt;
}

void HookGlobalHotkey::setDebouncedTime(size_t millisecond)
{
    debouncedTime_ = millisecond;
}

void HookGlobalHotkey::addPressedKey_(int key)
{
    std::lock_guard<std::mutex> lock(mtxListenKeyChanged_);

    if (key == VK_LWIN || key == VK_RWIN)
    {
        pressed_.addModifier(META);
    }
    else if (key == VK_MENU || key == VK_LMENU || key == VK_RMENU)
    {
        pressed_.addModifier(ALT);
    }
    else if (key == VK_CONTROL || key == VK_LCONTROL || key == VK_RCONTROL)
    {
        pressed_.addModifier(CTRL);
    }
    else if (key == VK_SHIFT || key == VK_LSHIFT || key == VK_RSHIFT)
    {
        pressed_.addModifier(SHIFT);
    }
    else
    {
        pressed_.setKey(key);
    }
}

void HookGlobalHotkey::removePressedKey_(int key)
{
    std::lock_guard<std::mutex> lock(mtxListenKeyChanged_);

    if (key == VK_LWIN || key == VK_RWIN)
    {
        pressed_.removeModifier(META);
    }
    else if (key == VK_MENU || key == VK_LMENU || key == VK_RMENU)
    {
        pressed_.removeModifier(ALT);
    }
    else if (key == VK_CONTROL || key == VK_LCONTROL || key == VK_RCONTROL)
    {
        pressed_.removeModifier(CTRL);
    }
    else if (key == VK_SHIFT || key == VK_LSHIFT || key == VK_RSHIFT)
    {
        pressed_.removeModifier(SHIFT);
    }
    else
    {
        pressed_.setKey(0);
    }
}

} // namespace gbhk

#endif // GLOBAL_HOTKEY_WIN && !GLOBAL_HOTKEY_NOHOOK
