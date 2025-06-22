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
    int doBeforeLoop() override;
    int doAfterLoop() override;

    void work() override;
    int workOfEnd() override;
    int workOfAdd(const KeyCombination& kc, bool autoRepeat) override;
    int workOfRemove(const KeyCombination& kc) override;
    int workOfRemoveAll() override;
    int workOfReplace(const KeyCombination& oldKc, const KeyCombination& newKc) override;
    int workOfSetAutoRepeat(const KeyCombination& kc, bool autoRepeat) override;

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

    void keyPressedCallback(int x11Keycode, int x11Modifiers);
    void keyReleasedCallback(int x11Keycode, int x11Modifiers);

    static std::unordered_map<int, int> keycodeTokeysym;

    KeyCombination prevKc;
    KeyCombination currKc;
    Display* display = NULL;
    XEvent event = {0};
};

} // namespace gbhk

#endif // _GLOBAL_HOTKEY_LINUX

#endif // !GLOBAL_HOTKEY_DISABLE_REGISTER

#endif // !GLOBAL_HOTKEY_REGISTER_GHM_PRIVATE_LINUX_HPP
