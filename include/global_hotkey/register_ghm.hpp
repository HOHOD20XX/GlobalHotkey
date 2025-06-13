#ifndef GLOBAL_HOTKEY_REGISTER_GHM_HPP
#define GLOBAL_HOTKEY_REGISTER_GHM_HPP

#ifndef GLOBAL_HOTKEY_DISABLE_REGISTER

#include "global_hotkey_manager.hpp"

namespace gbhk
{

class GBHK_API RegisterGHM final : public GlobalHotkeyManager
{
public:
    static RegisterGHM& getInstance();

    int start() override;
    int end() override;
    int add(const KeyCombination& kc, VoidFunc func, bool autoRepeat) override;
    int add(const KeyCombination& kc, ArgFunc func, Arg arg, bool autoRepeat) override;
    int remove(const KeyCombination& kc) override;
    int removeAll() override;
    int replace(const KeyCombination& oldKc, const KeyCombination& newKc) override;
    int setAutoRepeat(const KeyCombination& kc, bool autoRepeat) override;
    int setCycleTime(size_t milliseconds) override;
    bool has(const KeyCombination& kc) const override;
    bool isAutoRepeat(const KeyCombination& kc) const override;
    bool isRunning() const override;

private:
    RegisterGHM();
    ~RegisterGHM();
};

} // namespace gbhk

#endif // !GLOBAL_HOTKEY_DISABLE_REGISTER

#endif // !GLOBAL_HOTKEY_REGISTER_GHM_HPP
