#ifndef GLOBAL_HOTKEY_REGISTER_GHM_PRIVATE_MAC_HPP
#define GLOBAL_HOTKEY_REGISTER_GHM_PRIVATE_MAC_HPP

#ifndef GLOBAL_HOTKEY_DISABLE_REGISTER

#include "register_ghm_private.hpp"

#ifdef _GLOBAL_HOTKEY_MAC

namespace gbhk
{

class _RegisterGHMPrivateMac final : public _RegisterGHMPrivate
{
public:
    _RegisterGHMPrivateMac();
    ~_RegisterGHMPrivateMac();

private:
    void work() override;
    int workOfEnd() override;
    int workOfAdd(const KeyCombination& kc, bool autoRepeat) override;
    int workOfRemove(const KeyCombination& kc) override;
    int workOfRemoveAll() override;
    int workOfReplace(const KeyCombination& oldKc, const KeyCombination& newKc) override;
    int workOfSetAutoRepeat(const KeyCombination& kc, bool autoRepeat) override;
};

} // namespace gbhk

#endif // _GLOBAL_HOTKEY_MAC

#endif // !GLOBAL_HOTKEY_DISABLE_REGISTER

#endif // !GLOBAL_HOTKEY_REGISTER_GHM_PRIVATE_MAC_HPP
