#ifndef HOOK_GLOBAL_HOTKEY_HPP
#define HOOK_GLOBAL_HOTKEY_HPP

#include "global_hotkey_base.hpp"

// Only usable in windows platform.
#ifdef GBHK_WIN

#include <atomic>
#include <mutex>
#include <unordered_map>

namespace gbhk
{

class HookGlobalHotkey final : public GlobalHotkeyBase
{
public:
    static HookGlobalHotkey& getInstance();

    GBHK_NODISCARD uint start();
    GBHK_NODISCARD uint end();
    GBHK_NODISCARD uint add(const KeyCombination& keycomb, VoidFunc callbackFunc);
    GBHK_NODISCARD uint add(const KeyCombination& keycomb, ArgFunc callbackFunc, Arg arg);
    GBHK_NODISCARD uint remove(const KeyCombination& keycomb);
    GBHK_NODISCARD uint replace(const KeyCombination& oldKeycomb, const KeyCombination& newKeycomb);
    void setDebouncedTime(ullong millisecond);

private:
    HookGlobalHotkey();
    ~HookGlobalHotkey();
    HookGlobalHotkey(const HookGlobalHotkey& other) = delete;
    HookGlobalHotkey& operator=(const HookGlobalHotkey& other) = delete;

    static void addPressedKey_(uint key);
    static void removePressedKey_(uint key);

    static std::mutex mtxListenKeyChanged_;
    static KeyCombination pressed_;

    std::atomic<ullong> debouncedTime_;
    std::mutex mtxFuncsOperate_;
    std::unordered_map<KeyCombination, std::pair<bool, VoidFunc>, KeyCombination::Hash> voidFuncs_;
    std::unordered_map<KeyCombination, std::pair<bool, ArgFuncArg>, KeyCombination::Hash> argFuncArgs_;
};

}

#endif // GBHK_WIN

#endif // !HOOK_GLOBAL_HOTKEY_HPP
