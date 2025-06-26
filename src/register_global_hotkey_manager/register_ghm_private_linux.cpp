#ifndef GLOBAL_HOTKEY_DISABLE_REGISTER

#include "register_ghm_private_linux.hpp"

#ifdef _GLOBAL_HOTKEY_LINUX

#include <global_hotkey/return_code.hpp>

#include "../key/key_private_x11.hpp"

#define REGISTER_ATOM_NAME "Register"
#define UNREGISTER_ATOM_NAME "Unregister"
#define REGISTER_ATOM(display) XInternAtom(display, REGISTER_ATOM_NAME, False);
#define UNREGISTER_ATOM(display) XInternAtom(display, UNREGISTER_ATOM_NAME, False);

namespace gbhk
{

std::unordered_map<int, int> _RegisterGHMPrivateLinux::keycodeTokeysym;

_RegisterGHMPrivateLinux::_RegisterGHMPrivateLinux() : rc(RC_NOT_USED) {}

_RegisterGHMPrivateLinux::~_RegisterGHMPrivateLinux() { end(); }

int _RegisterGHMPrivateLinux::doBeforeThreadEnd()
{
    ErrorHandler eh;
    Window rootWindow = DefaultRootWindow(display);
    XDestroyWindow(display, rootWindow);
    return eh.errorCode;
}

void _RegisterGHMPrivateLinux::work()
{
    ErrorHandler eh;
    display = XOpenDisplay(NULL);
    if (eh.errorCode != RC_SUCCESS)
    {
        setFailedRunning(eh.errorCode);
        return;
    }

    Window rootWindow = DefaultRootWindow(display);
    auto mask = KeyPressMask | KeyReleaseMask | StructureNotifyMask;
    XSelectInput(display, rootWindow, mask);
    setSuccessRunning();

    KeyCombination prevKc;
    KeyCombination currKc;
    XEvent event = {0};
    while (true)
    {
        XPeekEvent(display, &event);
        if (event.type == KeyPress || event.type == KeyRelease)
        {
            if (event.type == KeyPress)
                currKc = keyPressedCallback(currKc, event.xkey.keycode, event.xkey.state);
            else
                currKc = keyReleasedCallback(currKc, event.xkey.keycode, event.xkey.state);

            invoke(prevKc, currKc);
        }
        else if (event.type == ClientMessage)
        {
            if (event.xclient.message_type == REGISTER_ATOM)
            {
                auto value = event.xclient.data.l[0];
                rc = nativeRegisterHotkey(KeyCombination::fromCombinedValue(value));
                cvReturned.notify_one();
            }
            else if (event.xclient.message_type == UNREGISTER_ATOM)
            {
                auto value = event.xclient.data.l[0];
                rc = nativeUnregisterHotkey(KeyCombination::fromCombinedValue(value));
                cvReturned.notify_one();
            }
        }
        else if (event.type == DestroyNotify)
        {
            break;
        }
    }

    XCloseDisplay(display);
    keycodeTokeysym.clear();
    prevKc = KeyCombination();
    currKc = KeyCombination();
    display = NULL;
}

int _RegisterGHMPrivateLinux::registerHotkey(const KeyCombination& kc, bool autoRepeat)
{
    rc = RC_NOT_USED;

    XEvent event;
    event.type = ClientMessage;
    Window rootWindow = DefaultRootWindow(display);
    event.xclient.window = rootWindow;
    event.xclient.message_type = REGISTER_ATOM;
    event.xclient.format = 32;
    event.xclient.data.l[0] = kc.combinedValue();

    ErrorHandler eh;
    XSendEvent(display, rootWindow, False, NoEventMask, &event);
    if (eh.errorCode == RC_SUCCESS)
    {
        std::mutex dummyLock;
        std::unique_lock<std::mutex> lock(dummyLock);
        cvReturned.wait(lock, [this]() { return (rc != RC_NOT_USED); });
        return rc;
    }
    return eh.errorCode;
}

int _RegisterGHMPrivateLinux::unregisterHotkey(const KeyCombination& kc)
{
    rc = RC_NOT_USED;

    XEvent event;
    event.type = ClientMessage;
    Window rootWindow = DefaultRootWindow(display);
    event.xclient.window = rootWindow;
    event.xclient.message_type = UNREGISTER_ATOM;
    event.xclient.format = 32;
    event.xclient.data.l[0] = kc.combinedValue();

    ErrorHandler eh;
    XSendEvent(display, rootWindow, False, NoEventMask, &event);
    if (eh.errorCode == RC_SUCCESS)
    {
        std::mutex dummyLock;
        std::unique_lock<std::mutex> lock(dummyLock);
        cvReturned.wait(lock, [this]() { return (rc != RC_NOT_USED); });
        return rc;
    }
    return eh.errorCode;
}

void _RegisterGHMPrivateLinux::invoke(const KeyCombination& prevKc, const KeyCombination& currKc)
{
    auto pair = getValue(currKc);
    auto& autoRepeat = pair.first;
    auto& fn = pair.second;
    bool shouldInvoke = fn && (currKc != prevKc || autoRepeat);
    if (shouldInvoke)
        fn();
    prevKc = currKc;
}

int _RegisterGHMPrivateLinux::nativeRegisterHotkey(const KeyCombination& kc)
{
    ErrorHandler eh;

    auto keysym = x11Keysym(kc.key());
    auto keycode = XKeysymToKeycode(display, keysym);
    keycodeTokeysym[keycode] = keysym;
    auto mod = x11Modifiers(kc.modifiers());
    XGrabKey(display, keycode, mod, DefaultRootWindow(display), True, GrabModeAsync, GrabModeAsync);
    XSync(display, False);

    return eh.errorCode;
}

int _RegisterGHMPrivateLinux::nativeUnregisterHotkey(const KeyCombination& kc)
{
    auto keycode = XKeysymToKeycode(display, x11Keysym(kc.key()));
    auto mod = x11Modifiers(kc.modifiers());
    XUngrabKey(display, keycode, mod, DefaultRootWindow(display));
    XSync(display, False);
    return RC_SUCCESS;
}

KeyCombination _RegisterGHMPrivateLinux::keyPressedCallback(int x11Keycode, int x11Modifiers)
{
    auto mod = getModifiersFromX11Modifiers(x11Modifiers);
    auto keysym = keycodeTokeysym[x11Keycode];
    auto key = getKeyFromX11Keysym(keysym);
    return {mod, key};
}

KeyCombination _RegisterGHMPrivateLinux::keyReleasedCallback(int x11Keycode, int x11Modifiers)
{
    return KeyCombination();
}

int _RegisterGHMPrivateLinux::ErrorHandler::errorCode = RC_SUCCESS;
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
