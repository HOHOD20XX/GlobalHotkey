#ifndef GLOBAL_HOTKEY_DISABLE_REGISTER

#include "register_ghm_private_linux.hpp"

#ifdef _GLOBAL_HOTKEY_LINUX

#include <global_hotkey/return_code.hpp>
#include <global_hotkey/utility.hpp>

#include "../key/key_private_x11.hpp"

namespace gbhk
{

std::unordered_map<int, int> _RegisterGHMPrivateLinux::keycodeTokeysym;

_RegisterGHMPrivateLinux::_RegisterGHMPrivateLinux() = default;

_RegisterGHMPrivateLinux::~_RegisterGHMPrivateLinux() { end(); }

int _RegisterGHMPrivateLinux::doBeforeLoop()
{
    ErrorHandler eh;
    display = XOpenDisplay(NULL);
    return eh.errorCode;
}

int _RegisterGHMPrivateLinux::doAfterLoop()
{
    ErrorHandler eh;
    XCloseDisplay(display);

    keycodeTokeysym.clear();
    prevKc = KeyCombination();
    currKc = KeyCombination();
    display = nullptr;
    event = {0};

    return eh.errorCode;
}

void _RegisterGHMPrivateLinux::work()
{
    while (XPending(display))
    {
        XNextEvent(display, &event);
        switch (event.type)
        {
            case KeyPress:
                keyPressedCallback(event.xkey.keycode, event.xkey.state);
                break;
            case KeyRelease:
                keyReleasedCallback(event.xkey.keycode, event.xkey.state);
                break;
            default:
                break;
        }
    }

    auto pair = getValue(currKc);
    auto& autoRepeat = pair.first;
    auto& fn = pair.second;
    bool shouldInvoke = fn && (currKc != prevKc || autoRepeat);
    if (shouldInvoke)
        fn();
    prevKc = currKc;
}

int _RegisterGHMPrivateLinux::workOfEnd()
{
    return workOfRemoveAll();
}

int _RegisterGHMPrivateLinux::workOfAdd(const KeyCombination& kc, bool autoRepeat)
{
    ErrorHandler eh;

    auto keysym = x11Keysym(kc.key());
    auto keyCode = XKeysymToKeycode(display, keysym);
    keycodeTokeysym[keyCode] = keysym;
    auto mod = x11Modifiers(kc.modifiers());
    XGrabKey(display, keyCode, mod, DefaultRootWindow(display), True, GrabModeAsync, GrabModeAsync);
    XSync(display, False);

    return eh.errorCode;
}

int _RegisterGHMPrivateLinux::workOfRemove(const KeyCombination& kc)
{
    ErrorHandler eh;

    auto keyCode = XKeysymToKeycode(display, x11Keysym(kc.key()));
    auto mod = x11Modifiers(kc.modifiers());
    XUngrabKey(display, keyCode, mod, DefaultRootWindow(display));
    XSync(display, False);

    // Always is RC_SUCCESS actually.
    return eh.errorCode;
}

int _RegisterGHMPrivateLinux::workOfRemoveAll()
{
    auto kcs = getAllKeyCombination();
    for (const auto& kc : kcs)
        int rc = workOfRemove(kc);
    return RC_SUCCESS;
}

int _RegisterGHMPrivateLinux::workOfReplace(const KeyCombination& oldKc, const KeyCombination& newKc)
{
    bool autoRepeat = isAutoRepeat(oldKc);
    int rc = workOfRemove(oldKc);
    if (rc != RC_SUCCESS)
        return rc;
    rc = workOfAdd(newKc, autoRepeat);
    return rc;
}

int _RegisterGHMPrivateLinux::workOfSetAutoRepeat(const KeyCombination& kc, bool autoRepeat)
{
    int rc = workOfRemove(kc);
    if (rc != RC_SUCCESS)
        return rc;
    rc = workOfAdd(kc, autoRepeat);
    return rc;
}

void _RegisterGHMPrivateLinux::keyPressedCallback(int x11Keycode, int x11Modifiers)
{
    auto keysym = keycodeTokeysym[x11Keycode];
    auto key = getKeyFromX11Keysym(keysym);
    auto mod = getModifiersFromX11Modifiers(x11Modifiers);
    currKc = {mod, key};
}

void _RegisterGHMPrivateLinux::keyReleasedCallback(int x11Keycode, int x11Modifiers)
{
    currKc = KeyCombination();
}

int _RegisterGHMPrivateLinux::ErrorHandler::errorCode = Success;
XErrorHandler _RegisterGHMPrivateLinux::ErrorHandler::prevXErrorHandler;

_RegisterGHMPrivateLinux::ErrorHandler::ErrorHandler()
{
    prevXErrorHandler = XSetErrorHandler(&_RegisterGHMPrivateLinux::ErrorHandler::handleError);
}

_RegisterGHMPrivateLinux::ErrorHandler::~ErrorHandler()
{
    XSetErrorHandler(prevXErrorHandler);
    errorCode = RC_SUCCESS;
}

int _RegisterGHMPrivateLinux::ErrorHandler::handleError(Display* display, XErrorEvent* error)
{
    if (error->error_code != Success)
        errorCode = error->error_code;
    errorCode = RC_SUCCESS;
    return errorCode;
}

} // namespace gbhk

#endif // _GLOBAL_HOTKEY_LINUX

#endif // !GLOBAL_HOTKEY_DISABLE_REGISTER
