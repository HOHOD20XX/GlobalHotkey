#ifndef GLOBAL_HOTKEY_REGISTER_GHM_PRIVATE_LINUX_HPP
#define GLOBAL_HOTKEY_REGISTER_GHM_PRIVATE_LINUX_HPP

#ifndef GLOBAL_HOTKEY_DISABLE_REGISTER

#include "register_ghm_private.hpp"

#ifdef _GLOBAL_HOTKEY_LINUX

#include <X11/Xlib.h>
#include <X11/Xutil.h>

namespace gbhk
{

class _RegisterGHMPrivateLinux final : public _RegisterGHMPrivate
{
public:
    _RegisterGHMPrivateLinux();
    ~_RegisterGHMPrivateLinux();

private:
    int doBeforeLoop() override;
    int doAfterLoop() override;

    void work() override;
    int workOfEnd() override;
    int workOfAdd(const KeyCombination& kc, bool autoRepeat) override;
    int workOfRemove(const KeyCombination& kc) override;
    int workOfRemoveAll() override;
    int workOfReplace(const KeyCombination& oldKc, const KeyCombination& newKc) override;
    int workOfSetAutoRepeat(const KeyCombination& kc, bool autoRepeat) override;

    Display* display;
};

} // namespace gbhk

#endif // _GLOBAL_HOTKEY_LINUX

#endif // !GLOBAL_HOTKEY_DISABLE_REGISTER

#endif // !GLOBAL_HOTKEY_REGISTER_GHM_PRIVATE_LINUX_HPP
