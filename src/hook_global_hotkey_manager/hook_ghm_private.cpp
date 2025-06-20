#ifndef GLOBAL_HOTKEY_DISABLE_HOOK

#include "hook_ghm_private.hpp"

#include "../key/key_private.hpp"

#include <global_hotkey/return_code.hpp>

namespace gbhk
{

std::atomic<Modifiers> _HookGHMPrivate::pressedMod(0);
std::atomic<Key> _HookGHMPrivate::pressedKey(0);

_HookGHMPrivate::_HookGHMPrivate() : kbhm(kbhook::KeyboardHookManager::getInstance()) {}

_HookGHMPrivate::~_HookGHMPrivate() { end(); }

int _HookGHMPrivate::specializedStart()
{
    int rc = kbhm.start();
    if (rc != RC_SUCCESS)
        return rc;
    kbhm.setKeyPressedEvent(&pressedKeyCallback);
    kbhm.setKeyReleasedEvent(&releasedKeyCallback);
    return RC_SUCCESS;
}

int _HookGHMPrivate::specializedEnd()
{
    int rc = kbhm.end();
    pressedMod = 0;
    pressedKey = 0;
    prevKc = KeyCombination();
    return rc;
}

void _HookGHMPrivate::eachCycleDo()
{
    KeyCombination kc(pressedMod, pressedKey);
    if (has(kc))
    {
        auto& pair = getValue(kc);
        auto& autoRepeat = pair.first;
        auto& fn = pair.second;
        bool shouldInvoke = fn && (kc != prevKc || autoRepeat);
        if (shouldInvoke)
            fn();
    }
    prevKc = kc;
}

void _HookGHMPrivate::pressedKeyCallback(int nativeKey)
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

void _HookGHMPrivate::releasedKeyCallback(int nativeKey)
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
