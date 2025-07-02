#ifndef DEFS_HPP
#define DEFS_HPP

#include <global_hotkey/global_hotkey.hpp>

using namespace gbhk;

#ifdef GLOBAL_HOTKEY_EXAMPLE_USE_HOOK
    #define GHM HookGlobalHotkeyManager::getInstance()
#else
    #define GHM RegisterGlobalHotkeyManager::getInstance()
#endif

#define RCMSG(rc)   returnCodeMessage(rc).c_str()
#define KCSTR(kc)   kc.toString().c_str()

constexpr KeyCombination exitKc(CTRL | SHIFT, Key_Backspace);
constexpr KeyCombination addKc(CTRL, 'A');
constexpr KeyCombination removeKc(CTRL, 'D');
constexpr KeyCombination replaceKc(CTRL, 'R');
constexpr KeyCombination listKc(CTRL, 'L');

constexpr KeyCombination simpleWorkKc(ALT, 'S');
constexpr KeyCombination heavyWorkKc(ALT, 'H');
constexpr KeyCombination clearTerminalKc(ALT, 'C');
constexpr KeyCombination setHeavyWorkLevelKc(CTRL | ALT, 'H');

#endif // !DEFS_HPP
