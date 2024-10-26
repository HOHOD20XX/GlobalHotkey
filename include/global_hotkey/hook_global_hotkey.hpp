#ifndef HOOK_GLOBAL_HOTKEY_HPP
#define HOOK_GLOBAL_HOTKEY_HPP

#include "global_hotkey_base.hpp"

// Only usable in windows platform.
#ifdef GBHK_WIN

#include <mutex>
#include <unordered_set>
#include <unordered_map>

namespace gbhk
{

class HookGlobalHotkey final : public GlobalHotkeyBase
{
public:
    static HookGlobalHotkey& getInstance();

    GBHK_NODISCARD uint start();
    GBHK_NODISCARD uint end();
    GBHK_NODISCARD uint add(const KeyCombination& keycomb, VoidFunc func);
    GBHK_NODISCARD uint add(const KeyCombination& keycomb, ArgFunc func, Arg arg);
    GBHK_NODISCARD uint remove(const KeyCombination& keycomb);
    GBHK_NODISCARD uint replace(const KeyCombination& oldKeycomb, const KeyCombination& newKeycomb);

private:
    HookGlobalHotkey();
    ~HookGlobalHotkey();
    HookGlobalHotkey(const HookGlobalHotkey& other) = delete;
    HookGlobalHotkey& operator=(const HookGlobalHotkey& other) = delete;

    KeyCombination getKeyCombination_();

    static void addPressedKey_(uint key);
    static void removePressedKey_(uint key);

    static std::mutex mtx_;
    static std::unordered_set<uint> pressedKeys_;

    std::unordered_map<KeyCombination, std::pair<bool, VoidFunc>,
        KeyCombination::Hash> voidFuncs_;
    std::unordered_map<KeyCombination, std::pair<bool, ArgFuncArg>,
        KeyCombination::Hash> argFuncArgs_;
};

}

#endif // GBHK_WIN

#endif // !HOOK_GLOBAL_HOTKEY_HPP
