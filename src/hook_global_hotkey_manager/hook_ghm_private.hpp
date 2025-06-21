#ifndef GLOBAL_HOTKEY_HOOK_GHM_PRIVATE_HPP
#define GLOBAL_HOTKEY_HOOK_GHM_PRIVATE_HPP

#ifndef GLOBAL_HOTKEY_DISABLE_HOOK

#include <global_hotkey/keyboard_hook_manager.hpp>

#include "../global_hotkey_manager/ghm_private.hpp"

namespace gbhk
{

class _HookGHMPrivate final : public _GHMPrivate
{
public:
    _HookGHMPrivate();
    ~_HookGHMPrivate();

protected:
    int specializedStart() override;
    int specializedEnd() override;

    void eachCycleDo() override;

private:
    static void keyPressedCallback(int nativeKey);
    static void keyReleasedCallback(int nativeKey);

    static std::atomic<Modifiers> pressedMod;
    static std::atomic<Key> pressedKey;

    KeyCombination prevKc;
    kbhook::KeyboardHookManager& kbhm;
};

} // namespace gbhk

#endif // !GLOBAL_HOTKEY_DISABLE_HOOK

#endif // !GLOBAL_HOTKEY_HOOK_GHM_PRIVATE_HPP
