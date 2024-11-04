#include <global_hotkey/hook_global_hotkey.hpp>

// Only usable in windows platform.
#ifdef _GLOBAL_HOTKEY_WIN

#include <chrono>

#include <global_hotkey/keyboard_hook.hpp>

#include <Windows.h>

namespace gbhk
{

std::mutex HookGlobalHotkey::mtxListenKeyChanged_;
KeyCombination HookGlobalHotkey::pressed_;

HookGlobalHotkey::HookGlobalHotkey() :
    debouncedTime_(0)
{}

HookGlobalHotkey::~HookGlobalHotkey()
{
    uint rslt = end();
}

HookGlobalHotkey& HookGlobalHotkey::getInstance()
{
    static HookGlobalHotkey instance;
    return instance;
}

GBHK_NODISCARD uint HookGlobalHotkey::start()
{
    namespace chr = std::chrono;

    // If is running do nothing.
    if (isRunning_)
        return _RC_UNTIMELY_CALL;

    // Set the callback function of key pressed and key released.
    keyboard_hook::setKeyPressedCallback(&addPressedKey_);
    keyboard_hook::setKeyReleaseddCallback(&removePressedKey_);
    // Start hook the #LowKeyboardEvent.
    uint rslt = keyboard_hook::run();
    // If failed to hook return error code.
    if (rslt != _RC_SUCCESS)
        return rslt;

    isRunning_ = true;
    workThread_ = std::thread([&] () {
        // Get the current thread id and set the #workThreadId_ value.
        setWorkThreadId_(std::this_thread::get_id());

        // Record the previous pressed key combination.
        KeyCombination prevKeycomb;
        chr::steady_clock::time_point prevWorkTime = chr::steady_clock::now();
        while (!shouldClose_) {
            setTimePoint_();

            // Get the current pressed key combination.
            mtxListenKeyChanged_.lock();
            KeyCombination keycomb = pressed_;
            mtxListenKeyChanged_.unlock();

            bool isPass = false;
            if (debouncedTime_ == 0) {
                isPass = true;
            } else {
                bool isJustModDiff = keycomb.key() == prevKeycomb.key() &&
                    keycomb.modifier() != prevKeycomb.modifier();
                bool isPrevModContainsCurMod = prevKeycomb.modifier() > keycomb.modifier() &&
                    ((prevKeycomb.modifier() & keycomb.modifier()) == keycomb.modifier());
                bool isInDebouncedTime = (chr::steady_clock::now() - prevWorkTime) <=
                    chr::milliseconds(debouncedTime_);
                if (!isJustModDiff || !isPrevModContainsCurMod || !isInDebouncedTime)
                    isPass = true;
            }

            if (isPass) {
                mtxFuncsOperate_.lock();

                if (voidFuncs_.find(keycomb) != voidFuncs_.end()) {
                    // If the current keycomb not equal to previous keycomb or hotkey is auto repeat execute related function.
                    if ((prevKeycomb != keycomb) || voidFuncs_[keycomb].first)
                        voidFuncs_[keycomb].second();
                } else if (argFuncArgs_.find(keycomb) != argFuncArgs_.end()) {
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

    return _RC_SUCCESS;
}

GBHK_NODISCARD uint HookGlobalHotkey::end()
{
    // If is not running do nothing.
    if (!isRunning_)
        return _RC_UNTIMELY_CALL;

    // This function can't called in the work thread, else program get stuck in a loop.
    if (std::this_thread::get_id() == getWorkThreadId_())
        return _RC_CALL_IN_WRONG_THREAD;

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

GBHK_NODISCARD uint HookGlobalHotkey::add(const KeyCombination& keycomb, VoidFunc callbackFunc)
{
    uint rslt = _RC_SUCCESS;

    mtxFuncsOperate_.lock();

    if (voidFuncs_.find(keycomb) != voidFuncs_.end())
        rslt = _RC_ALREADY_EXISTED;
    else
        voidFuncs_.insert({ keycomb, { keycomb.isAutoRepeat(), callbackFunc } });

    mtxFuncsOperate_.unlock();

    return rslt;
}

GBHK_NODISCARD uint HookGlobalHotkey::add(const KeyCombination& keycomb, ArgFunc callbackFunc, Arg arg)
{
    uint rslt = _RC_SUCCESS;

    mtxFuncsOperate_.lock();

    if (argFuncArgs_.find(keycomb) != argFuncArgs_.end())
        rslt = _RC_ALREADY_EXISTED;
    else
        argFuncArgs_.insert({ keycomb, { keycomb.isAutoRepeat(), { callbackFunc, arg } } });

    mtxFuncsOperate_.unlock();

    return rslt;
}

GBHK_NODISCARD uint HookGlobalHotkey::remove(const KeyCombination& keycomb)
{
    uint rslt = _RC_SUCCESS;

    mtxFuncsOperate_.lock();

    if (voidFuncs_.find(keycomb) == voidFuncs_.end() ||
        argFuncArgs_.find(keycomb) == argFuncArgs_.end()) {
        rslt = _RC_NOT_FIND;
    } else {
        voidFuncs_.erase(keycomb);
        argFuncArgs_.erase(keycomb);
    }

    mtxFuncsOperate_.unlock();

    return rslt;
}

GBHK_NODISCARD uint HookGlobalHotkey::replace(const KeyCombination& oldKeycomb,
                                              const KeyCombination& newKeycomb)
{
    // If the old key combination equal to new key combination do nothing.
    if (oldKeycomb.equal(newKeycomb))
        return _RC_OLD_EQUAL_NEW;

    uint rslt = _RC_SUCCESS;

    mtxFuncsOperate_.lock();

    if (voidFuncs_.find(oldKeycomb) == voidFuncs_.end() &&
        argFuncArgs_.find(oldKeycomb) == argFuncArgs_.end()) {
        rslt = _RC_NOT_FIND;
    } else if (voidFuncs_.find(oldKeycomb) != voidFuncs_.end()) {
        auto& func = voidFuncs_[oldKeycomb].second;
        voidFuncs_.erase(oldKeycomb);
        voidFuncs_.insert({ newKeycomb, { newKeycomb.isAutoRepeat(), func } });
    } else {
        auto& funcArg = argFuncArgs_[oldKeycomb].second;
        argFuncArgs_.erase(oldKeycomb);
        argFuncArgs_.insert({ newKeycomb, { newKeycomb.isAutoRepeat(), funcArg } });
    }

    mtxFuncsOperate_.unlock();

    return rslt;
}

void HookGlobalHotkey::setDebouncedTime(ullong millisecond)
{
    debouncedTime_ = millisecond;
}

void HookGlobalHotkey::addPressedKey_(uint key)
{
    mtxListenKeyChanged_.lock();

    if (key == VK_LWIN || key == VK_RWIN) {
        pressed_.addModifier(META);
    } else if (key == VK_MENU || key == VK_LMENU || key == VK_RMENU) {
        pressed_.addModifier(ALT);
    } else if (key == VK_CONTROL || key == VK_LCONTROL || key == VK_RCONTROL) {
        pressed_.addModifier(CTRL);
    } else if (key == VK_SHIFT || key == VK_LSHIFT || key == VK_RSHIFT) {
        pressed_.addModifier(SHIFT);
    } else {
        pressed_.setKey(key);
    }

    mtxListenKeyChanged_.unlock();
}

void HookGlobalHotkey::removePressedKey_(uint key)
{
    mtxListenKeyChanged_.lock();

    if (key == VK_LWIN || key == VK_RWIN) {
        pressed_.removeModifier(META);
    } else if (key == VK_MENU || key == VK_LMENU || key == VK_RMENU) {
        pressed_.removeModifier(ALT);
    } else if (key == VK_CONTROL || key == VK_LCONTROL || key == VK_RCONTROL) {
        pressed_.removeModifier(CTRL);
    } else if (key == VK_SHIFT || key == VK_LSHIFT || key == VK_RSHIFT) {
        pressed_.removeModifier(SHIFT);
    } else {
        pressed_.setKey(0);
    }

    mtxListenKeyChanged_.unlock();
}

}

#endif // _GLOBAL_HOTKEY_WIN
