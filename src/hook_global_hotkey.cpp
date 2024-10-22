#include <global_hotkey/hook_global_hotkey.hpp>
#include <global_hotkey/keyboard_hook.hpp>

// Only usable in windows platform.
#ifdef GBHK_WIN

#include <Windows.h>

namespace gbhk
{

std::mutex HookGlobalHotkey::mtx_;
std::unordered_set<uint> HookGlobalHotkey::pressedKeys_;

HookGlobalHotkey::HookGlobalHotkey() {}

HookGlobalHotkey::~HookGlobalHotkey()
{
    uint rslt = end();
}

HookGlobalHotkey &HookGlobalHotkey::getInstance()
{
    static HookGlobalHotkey instance;
    return instance;
}

GBHK_NODISCARD uint HookGlobalHotkey::start()
{
// If is running do nothing.
    if (isRunning_)
        return GBHK_RSLT_UNTIMELY_CALL;

    // Set the callback function of key pressed and key released.
    keyboard_hook::setKeyPressedCallback(&addPressedKey_);
    keyboard_hook::setKeyReleaseddCallback(&removePressedKey_);
    // Start hook the #LowKeyboardEvent.
    uint rslt = keyboard_hook::run();
    // If failed to hook return error code.
    if (rslt != GBHK_RSLT_SUCCESS)
        return rslt;

    isRunning_ = true;
    workThread_ = std::thread([&] () {
        // Get the current thread id and set the #workThreadId_ value.
        setWorkThreadId_(std::this_thread::get_id());

        // Record the previous pressed key combination.
        KeyCombination prevKeycomb;
        while (!shouldClose_) {
            // Get the current pressed key combination.
            KeyCombination keycomb = getKeyCombination_();

            mtx_.lock();

            if (voidFuncs_.find(keycomb) != voidFuncs_.end()) {
                // If the current keycomb not equal to previous keycomb or hotkey is auto repeat execute related function.
                if ((prevKeycomb != keycomb) || voidFuncs_[keycomb].first)
                    voidFuncs_[keycomb].second();
            } else if (argFuncArgs_.find(keycomb) != argFuncArgs_.end()) {
                auto &fnArg = argFuncArgs_[keycomb].second;
                // Ditto.
                if ((prevKeycomb != keycomb) || argFuncArgs_[keycomb].first)
                    fnArg.first(fnArg.second);
            }

            mtx_.unlock();

            prevKeycomb = keycomb;

            sleep_(delay_);
        }

        isRunning_ = false;
    });
    workThread_.detach();

    return GBHK_RSLT_SUCCESS;
}

GBHK_NODISCARD uint HookGlobalHotkey::end()
{
// If is not running do nothing.
    if (!isRunning_)
        return GBHK_RSLT_UNTIMELY_CALL;

    // Set this flag to true to exit thread.
    shouldClose_ = true;
    // Wait the thread exits.
    while (isRunning_)
        sleep_(1);
    // Reset this flag to default state.
    shouldClose_ = false;

    // Clear members.
    // Because the thread is exited, so not has thread safe problem.
    voidFuncs_.clear();
    argFuncArgs_.clear();

    return keyboard_hook::end();
}

GBHK_NODISCARD uint HookGlobalHotkey::add(const KeyCombination &keycomb, VoidFunc func)
{
    uint rslt = GBHK_RSLT_SUCCESS;

    mtx_.lock();

    if (voidFuncs_.find(keycomb) != voidFuncs_.end())
        rslt = GBHK_RSLT_ALREADY_EXISTED;
    else
        voidFuncs_.insert({keycomb, {keycomb.isAutoRepeat(), func}});

    mtx_.unlock();

    return rslt;
}

GBHK_NODISCARD uint HookGlobalHotkey::add(const KeyCombination &keycomb, ArgFunc func, Arg arg)
{
    uint rslt = GBHK_RSLT_SUCCESS;

    mtx_.lock();

    if (argFuncArgs_.find(keycomb) != argFuncArgs_.end())
        rslt = GBHK_RSLT_ALREADY_EXISTED;
    else
        argFuncArgs_.insert({keycomb, {keycomb.isAutoRepeat(), {func, arg}}});

    mtx_.unlock();

    return rslt;
}

GBHK_NODISCARD uint HookGlobalHotkey::remove(const KeyCombination &keycomb)
{
    uint rslt = GBHK_RSLT_SUCCESS;

    mtx_.lock();

    if (voidFuncs_.find(keycomb) == voidFuncs_.end() ||
        argFuncArgs_.find(keycomb) == argFuncArgs_.end()) {
        rslt = GBHK_RSLT_NOT_FIND;
    } else {
        voidFuncs_.erase(keycomb);
        argFuncArgs_.erase(keycomb);
    }

    mtx_.unlock();

    return rslt;
}

GBHK_NODISCARD uint HookGlobalHotkey::replace(const KeyCombination &oldKeycomb,
                                              const KeyCombination &newKeycomb)
{
// If the old key combination equal to new key combination do nothing.
    if (oldKeycomb.equal(newKeycomb))
        return GBHK_RSLT_OLD_EQUAL_NEW;

    uint rslt = GBHK_RSLT_SUCCESS;

    mtx_.lock();

    if (voidFuncs_.find(oldKeycomb) == voidFuncs_.end() &&
        argFuncArgs_.find(oldKeycomb) == argFuncArgs_.end()) {
        rslt = GBHK_RSLT_NOT_FIND;
    } else if (voidFuncs_.find(oldKeycomb) != voidFuncs_.end()) {
        auto &func = voidFuncs_[oldKeycomb].second;
        voidFuncs_.erase(oldKeycomb);
        voidFuncs_.insert({newKeycomb, {newKeycomb.isAutoRepeat(), func}});
    } else {
        auto &funcArg = argFuncArgs_[oldKeycomb].second;
        argFuncArgs_.erase(oldKeycomb);
        argFuncArgs_.insert({newKeycomb, {newKeycomb.isAutoRepeat(), funcArg}});
    }

    mtx_.unlock();

    return rslt;
}

KeyCombination HookGlobalHotkey::getKeyCombination_()
{
    KeyCombination keycomb;

    mtx_.lock();

    for (auto &var : pressedKeys_) {
        if (var == VK_LWIN || var == VK_RWIN) {
            keycomb.addModifier(META);
        } else if (var == VK_MENU || var == VK_LMENU || var == VK_RMENU) {
            keycomb.addModifier(ALT);
        } else if (var == VK_CONTROL || var == VK_LCONTROL || var == VK_RCONTROL) {
            keycomb.addModifier(CTRL);
        } else if (var == VK_SHIFT || var == VK_LSHIFT || var == VK_RSHIFT) {
            keycomb.addModifier(SHIFT);
        } else {
            keycomb.setKey(var);
            break;
        }
    }

    mtx_.unlock();

    return keycomb;
}

void HookGlobalHotkey::addPressedKey_(uint key)
{
    mtx_.lock();
    pressedKeys_.insert(key);
    mtx_.unlock();
}

void HookGlobalHotkey::removePressedKey_(uint key)
{
    mtx_.lock();
    pressedKeys_.erase(key);
    mtx_.unlock();
}

}

#endif // GBHK_WIN
