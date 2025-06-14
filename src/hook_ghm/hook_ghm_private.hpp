#ifndef GLOBAL_HOTKEY_HOOK_GHM_PRIVATE_HPP
#define GLOBAL_HOTKEY_HOOK_GHM_PRIVATE_HPP

#ifndef GLOBAL_HOTKEY_DISABLE_HOOK

#include "../global_hotkey_manager/global_hotkey_manager_private.hpp"

namespace gbhk
{

class _HookGHMPrivate final : public _GlobalHotkeyManagerPrivate
{
public:
    _HookGHMPrivate();
    ~_HookGHMPrivate();

    int start() override;
    int end() override;
    int add(const KeyCombination& kc, const std::function<void()>& func, bool autoRepeat) override;
    int add(const KeyCombination& kc, std::function<void()>&& func, bool autoRepeat) override;
    int remove(const KeyCombination& kc) override;
    int removeAll() override;
    int replace(const KeyCombination& oldKc, const KeyCombination& newKc) override;
    int setAutoRepeat(const KeyCombination& kc, bool autoRepeat) override;
    int setDebouncedTime(size_t milliseconds);

private:
    void work_();

    static void pressedKeyCallback_(int nativeKey);
    static void releasedKeyCallback_(int nativeKey);

    static std::atomic<Modifiers> pressedModi_;
    static std::atomic<Key> pressedKey_;

    std::atomic<size_t> debouncedTime_;
};

} // namespace gbhk

#endif // !GLOBAL_HOTKEY_DISABLE_HOOK

#endif // !GLOBAL_HOTKEY_HOOK_GHM_PRIVATE_HPP
