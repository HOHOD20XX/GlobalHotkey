#ifndef GLOBAL_HOTKEY_REGISTER_GHM_PRIVATE_X11_HPP
#define GLOBAL_HOTKEY_REGISTER_GHM_PRIVATE_X11_HPP

#ifndef GLOBAL_HOTKEY_DISABLE_REGISTER

#include "register_ghm_private.hpp"

#ifdef _GLOBAL_HOTKEY_LINUX

#include <X11/Xlib.h>

namespace gbhk
{

class _RegisterGHMPrivateX11 final : public _RegisterGHMPrivate
{
public:
    _RegisterGHMPrivateX11();
    ~_RegisterGHMPrivateX11();

protected:
    int doBeforeThreadRun() override;
    int doBeforeThreadEnd() override;
    void work() override;
    int registerHotkey(const KeyCombination& kc, bool autoRepeat) override;
    int unregisterHotkey(const KeyCombination& kc) override;

private:
    // The follow functions only be called in worker thread.
    void invoke(const KeyCombination& prevKc, const KeyCombination& currKc);
    int nativeRegisterHotkey(Display* display);
    int nativeUnregisterHotkey(Display* display);

    std::condition_variable cvRegUnregRc;
    std::atomic<int> regUnregRc;
    std::atomic<KeyCombination> regUnregKc;
    int eventFd = -1;
};

} // namespace gbhk

#endif // _GLOBAL_HOTKEY_LINUX

#endif // !GLOBAL_HOTKEY_DISABLE_REGISTER

#endif // !GLOBAL_HOTKEY_REGISTER_GHM_PRIVATE_X11_HPP
