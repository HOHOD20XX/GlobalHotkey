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

    int start();
    int end();
    int add(const KeyCombination& kc, VoidFunc func, bool autoRepeat = false);
    int add(const KeyCombination& kc, ArgFunc func, Arg arg, bool autoRepeat = false);
    int remove(const KeyCombination& kc);
    int removeAll();
    int replace(const KeyCombination& oldKc, const KeyCombination& newKc);
    int setAutoRepeat(const KeyCombination& kc, bool autoRepeat);
    int setCycleTime(size_t milliseconds);
    bool has(const KeyCombination& kc) const;
    bool isAutoRepeat(const KeyCombination& kc) const;
    bool isRunning() const;

private:
    RegisterGHM();
    ~RegisterGHM();
};

} // namespace gbhk

#endif // !GLOBAL_HOTKEY_DISABLE_REGISTER

#endif // !GLOBAL_HOTKEY_REGISTER_GHM_HPP
