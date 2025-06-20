#ifndef GLOBAL_HOTKEY_REGISTER_GHM_PRIVATE_WIN_HPP
#define GLOBAL_HOTKEY_REGISTER_GHM_PRIVATE_WIN_HPP

#ifndef GLOBAL_HOTKEY_DISABLE_REGISTER

#include "register_ghm_private.hpp"

#ifdef _GLOBAL_HOTKEY_WIN

#include <windows.h>

namespace gbhk
{

class _RegisterGHMPrivateWin final : public _RegisterGHMPrivate
{
public:
    _RegisterGHMPrivateWin();
    ~_RegisterGHMPrivateWin();

private:
    void work() override;
    int workOfEnd() override;
    int workOfAdd(const KeyCombination& kc, bool autoRepeat) override;
    int workOfRemove(const KeyCombination& kc) override;
    int workOfRemoveAll() override;
    int workOfReplace(const KeyCombination& oldKc, const KeyCombination& newKc) override;
    int workOfSetAutoRepeat(const KeyCombination& kc, bool autoRepeat) override;

    MSG msg = {0};
    std::atomic<int> hotkeyIndex;
    std::unordered_map<int, KeyCombination> hotkeyIdToKc;
    std::unordered_map<KeyCombination, int> kcToHotkeyId;
};

} // namespace gbhk

#endif // _GLOBAL_HOTKEY_WIN

#endif // !GLOBAL_HOTKEY_DISABLE_REGISTER

#endif // !GLOBAL_HOTKEY_REGISTER_GHM_PRIVATE_WIN_HPP
