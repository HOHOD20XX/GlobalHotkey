#ifndef GLOBAL_HOTKEY_HOOK_GHM_PRIVATE_HPP
#define GLOBAL_HOTKEY_HOOK_GHM_PRIVATE_HPP

#ifndef GLOBAL_HOTKEY_DISABLE_HOOK

#include <global_hotkey/keyboard_hook_manager.hpp>

#include "../global_hotkey_manager/ghm_private.hpp"

namespace gbhk
{

template <typename T>
struct PriorityItem
{
    PriorityItem(const T& item, size_t priority) : item(item), priority(priority) {}

    friend constexpr inline bool operator<(const PriorityItem& lhs, const PriorityItem& rhs) noexcept
    { return lhs.priority < rhs.priority; }

    T item;
    size_t priority;
};

class _HookGHMPrivate final : public _GHMPrivate
{
public:
    _HookGHMPrivate();
    ~_HookGHMPrivate();

protected:
    int doBeforeThreadRun() override;
    int doBeforeThreadEnd() override;
    void work() override;
    int registerHotkey(const KeyCombination& kc, bool autoRepeat);
    int unregisterHotkey(const KeyCombination& kc);

private:
    void invoke(const KeyCombination& prevKc, const KeyCombination& currKc) const;

    kbhook::KeyboardHookManager& kbhm;
};

} // namespace gbhk

#endif // !GLOBAL_HOTKEY_DISABLE_HOOK

#endif // !GLOBAL_HOTKEY_HOOK_GHM_PRIVATE_HPP
