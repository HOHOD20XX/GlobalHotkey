#pragma once

#include <global_hotkey/global_hotkey.hpp>

#ifdef GLOBAL_HOTKEY_EXAMPLE_USE_HOOK
    #define GHM HookGlobalHotkeyManager::getInstance()
#else
    #define GHM RegisterGlobalHotkeyManager::getInstance()
#endif

using namespace gbhk;

constexpr KeyCombination exitKc(CTRL | SHIFT, Key_Backspace);
constexpr KeyCombination addKc(ALT, Key_A);
constexpr KeyCombination removeKc(ALT, Key_D);
constexpr KeyCombination replaceKc(ALT, Key_R);
constexpr KeyCombination easyWorkKc(ALT | SHIFT, Key_E);
constexpr KeyCombination heavyWorkKc(ALT | SHIFT, Key_H);
constexpr KeyCombination setHeavyWorkTimeKc(CTRL | ALT | SHIFT, Key_H);
