#ifndef GLOBAL_HOTKEY_DISABLE_HOOK

#include "hook_ghm_private.hpp"

#include <chrono>   // chrono
#include <thread>   // this_thread

#include <global_hotkey/return_code.hpp>

#include "../key/key_private.hpp"

namespace gbhk
{

std::atomic<Modifiers> _HookGHMPrivate::pressedMod(0);
std::atomic<Key> _HookGHMPrivate::pressedKey(0);

_HookGHMPrivate::_HookGHMPrivate() :
    shouldClose(false),
    kbhm(kbhook::KeyboardHookManager::getInstance())
{}

_HookGHMPrivate::~_HookGHMPrivate() { end(); }

int _HookGHMPrivate::doBeforeThreadRun()
{
    int rc = kbhm.run();
    if (rc != RC_SUCCESS)
        return rc;
    rc = kbhm.setKeyPressedCallback(&keyPressedCallback);
    if (rc != RC_SUCCESS)
        return rc;
    rc = kbhm.setKeyReleasedCallback(&keyReleasedCallback);
    if (rc != RC_SUCCESS)
        return rc;
    shouldClose = false;
    return rc;
}

int _HookGHMPrivate::doBeforeThreadEnd()
{
    int rc = kbhm.end();
    shouldClose = true;
    pressedMod = 0;
    pressedKey = 0;
    return rc;
}

void _HookGHMPrivate::work()
{
    setRunSuccess();

    KeyCombination prevKc;
    std::chrono::steady_clock::time_point startTime;
    while (!shouldClose)
    {
        startTime = std::chrono::steady_clock::now();

        KeyCombination currKc(pressedMod, pressedKey);
        auto pair = getPairValue(currKc);
        auto& autoRepeat = pair.first;
        auto& fn = pair.second;
        bool shouldInvoke = fn && (currKc != prevKc || autoRepeat);
        if (shouldInvoke)
            fn();
        prevKc = currKc;

        auto targetTime = startTime + std::chrono::milliseconds(10);
        std::this_thread::sleep_until(targetTime);
    }
}

int _HookGHMPrivate::registerHotkey(const KeyCombination& kc, bool autoRepeat)
{ return RC_SUCCESS; }

int _HookGHMPrivate::unregisterHotkey(const KeyCombination& kc)
{ return RC_SUCCESS; }

void _HookGHMPrivate::keyPressedCallback(int nativeKey)
{
    auto key = getKeyFromNativeKey(nativeKey);
    if (key == Key_Mod_Meta || key == Key_Mod_Meta_Left || key == Key_Mod_Meta_Right)
        pressedMod = pressedMod.load().add(META);
    else if (key == Key_Mod_Ctrl || key == Key_Mod_Ctrl_Left || key == Key_Mod_Ctrl_Right)
        pressedMod = pressedMod.load().add(CTRL);
    else if (key == Key_Mod_Alt || key == Key_Mod_Alt_Left || key == Key_Mod_Alt_Right)
        pressedMod = pressedMod.load().add(ALT);
    else if (key == Key_Mod_Shift || key == Key_Mod_Shift_Left || key == Key_Mod_Shift_Right)
        pressedMod = pressedMod.load().add(SHIFT);
    else
        pressedKey = key;
}

void _HookGHMPrivate::keyReleasedCallback(int nativeKey)
{
    auto key = getKeyFromNativeKey(nativeKey);
    if (key == Key_Mod_Meta || key == Key_Mod_Meta_Left || key == Key_Mod_Meta_Right)
        pressedMod = pressedMod.load().remove(META);
    else if (key == Key_Mod_Ctrl || key == Key_Mod_Ctrl_Left || key == Key_Mod_Ctrl_Right)
        pressedMod = pressedMod.load().remove(CTRL);
    else if (key == Key_Mod_Alt || key == Key_Mod_Alt_Left || key == Key_Mod_Alt_Right)
        pressedMod = pressedMod.load().remove(ALT);
    else if (key == Key_Mod_Shift || key == Key_Mod_Shift_Left || key == Key_Mod_Shift_Right)
        pressedMod = pressedMod.load().remove(SHIFT);
    else
        pressedKey = 0;
}

} // namespace gbhk

#endif // !GLOBAL_HOTKEY_DISABLE_HOOK
