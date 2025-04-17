#ifndef GLOBAL_HOTKEY_HOOK_GLOBAL_HOTKEY_HPP
#define GLOBAL_HOTKEY_HOOK_GLOBAL_HOTKEY_HPP

#include "global_hotkey_base.hpp"

#if defined(GLOBAL_HOTKEY_WIN) && !defined(GLOBAL_HOTKEY_NOHOOK)

#include <unordered_map>    // unordered_map

namespace gbhk
{

class GBHK_API HookGlobalHotkey final : public GlobalHotkeyBase
{
public:
    static HookGlobalHotkey& getInstance();

    GBHK_NODISCARD int start();
    GBHK_NODISCARD int end();
    GBHK_NODISCARD int add(const KeyCombination& keycomb, VoidFunc callbackFunc);
    GBHK_NODISCARD int add(const KeyCombination& keycomb, ArgFunc callbackFunc, Arg arg);
    GBHK_NODISCARD int remove(const KeyCombination& keycomb);
    GBHK_NODISCARD int replace(const KeyCombination& oldKeycomb, const KeyCombination& newKeycomb);
    void setDebouncedTime(size_t millisecond);

private:
    HookGlobalHotkey();
    ~HookGlobalHotkey();
    HookGlobalHotkey(const HookGlobalHotkey& other) = delete;
    HookGlobalHotkey& operator=(const HookGlobalHotkey& other) = delete;

    static void addPressedKey_(int key);
    static void removePressedKey_(int key);

    static std::mutex mtxListenKeyChanged_;
    static KeyCombination pressed_;

    std::atomic<size_t> debouncedTime_;
    std::mutex mtxFuncsOperate_;
    std::unordered_map<KeyCombination, std::pair<bool, VoidFunc>, KeyCombination::Hash> voidFuncs_;
    std::unordered_map<KeyCombination, std::pair<bool, ArgFuncArg>, KeyCombination::Hash> argFuncArgs_;
};

} // namespace gbhk

#endif // GLOBAL_HOTKEY_WIN && !GLOBAL_HOTKEY_NOHOOK

#endif // !GLOBAL_HOTKEY_HOOK_GLOBAL_HOTKEY_HPP
