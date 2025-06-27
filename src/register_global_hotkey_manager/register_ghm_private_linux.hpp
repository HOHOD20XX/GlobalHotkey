#ifndef GLOBAL_HOTKEY_REGISTER_GHM_PRIVATE_LINUX_HPP
#define GLOBAL_HOTKEY_REGISTER_GHM_PRIVATE_LINUX_HPP

#ifndef GLOBAL_HOTKEY_DISABLE_REGISTER

#include "register_ghm_private.hpp"

#ifdef _GLOBAL_HOTKEY_LINUX

#include <X11/Xlib.h>

namespace gbhk
{

class _RegisterGHMPrivateLinux final : public _RegisterGHMPrivate
{
public:
    _RegisterGHMPrivateLinux();
    ~_RegisterGHMPrivateLinux();

protected:
    int doBeforeThreadStart() override;
    int doBeforeThreadEnd() override;
    void work() override;
    int registerHotkey(const KeyCombination& kc, bool autoRepeat) override;
    int unregisterHotkey(const KeyCombination& kc) override;

private:
    class ErrorHandler
    {
    public:
        ErrorHandler();
        ~ErrorHandler();

        static int errorCode;

    private:
        static int handleError(Display* display, XErrorEvent* error);
        static XErrorHandler prevXErrorHandler;
    };

    // The follow functions only be called in worker thread.
    void invoke(const KeyCombination& prevKc, const KeyCombination& currKc);
    int nativeRegisterHotkey(Display* display, const KeyCombination& kc);
    int nativeUnregisterHotkey(Display* display, const KeyCombination& kc);

    KeyCombination keyPressedCallback(int x11Keycode, int x11Modifiers);
    KeyCombination keyReleasedCallback(int x11Keycode, int x11Modifiers);

    static std::unordered_map<int, int> keycodeTokeysym;

    std::condition_variable cvReturned;
    std::atomic<int> rc;
    std::atomic<KeyCombination> atomicKc;
    int fd;
};

} // namespace gbhk

#endif // _GLOBAL_HOTKEY_LINUX

#endif // !GLOBAL_HOTKEY_DISABLE_REGISTER

#endif // !GLOBAL_HOTKEY_REGISTER_GHM_PRIVATE_LINUX_HPP
