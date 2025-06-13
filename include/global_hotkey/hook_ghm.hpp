#ifndef GLOBAL_HOTKEY_HOOK_GHM_HPP
#define GLOBAL_HOTKEY_HOOK_GHM_HPP

#ifndef GLOBAL_HOTKEY_DISABLE_HOOK

#include "global_hotkey_manager.hpp"

namespace gbhk
{

class GBHK_API HookGHM final : public GlobalHotkeyManager
{
public:
    static HookGHM& getInstance();

    int start() override;
    int end() override;
    int add(const KeyCombination& kc, VoidFunc func, bool autoRepeat) override;
    int add(const KeyCombination& kc, ArgFunc func, Arg arg, bool autoRepeat) override;
    int remove(const KeyCombination& kc) override;
    int removeAll() override;
    int replace(const KeyCombination& oldKc, const KeyCombination& newKc) override;
    int setAutoRepeat(const KeyCombination& kc, bool autoRepeat) override;
    int setCycleTime(size_t milliseconds) override;
    int setDebouncedTime(size_t milliseconds);
    bool has(const KeyCombination& kc) const override;
    bool isAutoRepeat(const KeyCombination& kc) const override;
    bool isRunning() const override;

private:
    HookGHM();
    ~HookGHM();
};

} // namespace gbhk

#endif // !GLOBAL_HOTKEY_DISABLE_HOOK

#endif // !GLOBAL_HOTKEY_HOOK_GHM_HPP
