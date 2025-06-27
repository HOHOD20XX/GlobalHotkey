#ifndef GLOBAL_HOTKEY_DISABLE_REGISTER

#include "register_ghm_private_linux.hpp"

#ifdef _GLOBAL_HOTKEY_LINUX

#include <poll.h>
#include <sys/eventfd.h>
#include <unistd.h>

#include <global_hotkey/return_code.hpp>

#include "../key/key_private_x11.hpp"

#define ET_EXIT         1
#define ET_REGISTER     2
#define ET_UNREGISTER   3

namespace gbhk
{

// 8 byte for `write` and `read` of the fd created by `eventfd`.
using EventType = int64_t;

std::unordered_map<int, int> _RegisterGHMPrivateLinux::keycodeTokeysym;

_RegisterGHMPrivateLinux::_RegisterGHMPrivateLinux() : rc(RC_NOT_USED) {}

_RegisterGHMPrivateLinux::~_RegisterGHMPrivateLinux() { end(); }

int _RegisterGHMPrivateLinux::doBeforeThreadStart()
{
    fd = eventfd(0, 0);
    if (fd == -1)
        return errno;
    return RC_SUCCESS;
}

int _RegisterGHMPrivateLinux::doBeforeThreadEnd()
{
    EventType et = ET_EXIT;
    auto ret = write(fd, &et, 8);
    if (ret == -1)
        return errno;
    return RC_SUCCESS;
}

void _RegisterGHMPrivateLinux::work()
{
    ErrorHandler eh;
    Display* display = XOpenDisplay(NULL);
    if (eh.errorCode != RC_SUCCESS)
    {
        setFailedRunning(eh.errorCode);
        return;
    }

    int x11Fd = ConnectionNumber(display);
    pollfd fds[2];
    fds[0].fd = x11Fd;
    fds[0].events = POLLIN;
    fds[1].fd = fd;
    fds[1].events = POLLIN;

    setSuccessRunning();
    KeyCombination prevKc;
    KeyCombination currKc;
    XEvent event = {0};
    while (true)
    {
        int ret = poll(fds, 2, -1);
        if (ret == -1)
            continue;;

        // Get the XEvent.
        if (fds[0].revents & POLLIN)
        {
            while (XPending(display))
            {
                XNextEvent(display, &event);
                if (event.type == KeyPress)
                    currKc = keyPressedCallback(event.xkey.keycode, event.xkey.state);
                else if (event.type == KeyRelease)
                    currKc = keyReleasedCallback(event.xkey.keycode, event.xkey.state);
                invoke(prevKc, currKc);
                prevKc = currKc;
            }
        }

        // Get the Event.
        if (fds[1].revents & POLLIN)
        {
            EventType et;
            auto ret = read(fd, &et, 8);
            if (ret != 8)
                continue;

            if (et == ET_EXIT)
            {
                break;
            }
            else if (et == ET_REGISTER)
            {
                rc = nativeRegisterHotkey(display, atomicKc);
                cvReturned.notify_one();
            }
            else if (et == ET_UNREGISTER)
            {
                rc = nativeUnregisterHotkey(display, atomicKc);
                cvReturned.notify_one();
            }
        }
    }

    XCloseDisplay(display);
    keycodeTokeysym.clear();
    close(fd);
}

int _RegisterGHMPrivateLinux::registerHotkey(const KeyCombination& kc, bool autoRepeat)
{
    rc = RC_NOT_USED;

    atomicKc = kc;
    EventType et = ET_REGISTER;
    auto ret = write(fd, &et, 8);
    if (ret == -1)
        return errno;

    std::mutex dummyLock;
    std::unique_lock<std::mutex> lock(dummyLock);
    cvReturned.wait(lock, [this]() { return (rc != RC_NOT_USED); });
    return rc;
}

int _RegisterGHMPrivateLinux::unregisterHotkey(const KeyCombination& kc)
{
    rc = RC_NOT_USED;

    atomicKc = kc;
    EventType et = ET_UNREGISTER;
    auto ret = write(fd, &et, 8);
    if (ret != 8)
        return errno;

    std::mutex dummyLock;
    std::unique_lock<std::mutex> lock(dummyLock);
    cvReturned.wait(lock, [this]() { return (rc != RC_NOT_USED); });
    return rc;
}

void _RegisterGHMPrivateLinux::invoke(const KeyCombination& prevKc, const KeyCombination& currKc)
{
    auto pair = getValue(currKc);
    auto& autoRepeat = pair.first;
    auto& fn = pair.second;
    bool shouldInvoke = fn && (currKc != prevKc || autoRepeat);
    if (shouldInvoke)
        fn();
}

int _RegisterGHMPrivateLinux::nativeRegisterHotkey(Display* display, const KeyCombination& kc)
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

int _RegisterGHMPrivateLinux::nativeUnregisterHotkey(Display* display, const KeyCombination& kc)
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
