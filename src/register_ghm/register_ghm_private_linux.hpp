#ifndef GLOBAL_HOTKEY_REGISTER_GHM_PRIVATE_LINUX_HPP
#define GLOBAL_HOTKEY_REGISTER_GHM_PRIVATE_LINUX_HPP

#ifndef GLOBAL_HOTKEY_DISABLE_REGISTER

#include "register_ghm_private.hpp"

#ifdef _GLOBAL_HOTKEY_LINUX

namespace gbhk
{

class _RegisterGHMPrivateLinux final : public _RegisterGHMPrivate
{
public:
    _RegisterGHMPrivateLinux();
    ~_RegisterGHMPrivateLinux();

    int start() override;
    int end() override;
    int add(const KeyCombination& kc, const std::function<void()>& fn, bool autoRepeat) override;
    int add(const KeyCombination& kc, std::function<void()>&& fn, bool autoRepeat) override;
    int remove(const KeyCombination& kc) override;
    int removeAll() override;
    int replace(const KeyCombination& oldKc, const KeyCombination& newKc) override;
    int setAutoRepeat(const KeyCombination& kc, bool autoRepeat) override;

private:
};

} // namespace gbhk

#endif // _GLOBAL_HOTKEY_LINUX

#endif // !GLOBAL_HOTKEY_DISABLE_REGISTER

#endif // !GLOBAL_HOTKEY_REGISTER_GHM_PRIVATE_LINUX_HPP
