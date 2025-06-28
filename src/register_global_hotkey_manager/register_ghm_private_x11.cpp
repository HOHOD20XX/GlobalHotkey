#ifndef GLOBAL_HOTKEY_DISABLE_REGISTER

#include "register_ghm_private_x11.hpp"

#ifdef _GLOBAL_HOTKEY_LINUX

#include <poll.h>           // poll
#include <sys/eventfd.h>    // eventfd
#include <unistd.h>         // read, write, close

#include <global_hotkey/return_code.hpp>

#include "../key/key_private_x11.hpp"

namespace gbhk
{

class ErrorHandler
{
public:
    ErrorHandler();
    ~ErrorHandler();

    static int ec;

private:
    static int handleError(Display* display, XErrorEvent* error);
    static XErrorHandler prevXErrorHandler;
};

// 8 byte for `write` and `read` of the fd created by `eventfd`.
enum EventType : int64_t
{
    ET_EXIT = 1,
    ET_REGISTER,
    ET_UNREGISTER
};

static std::unordered_map<int, int> KeycodeToKeysym;

_RegisterGHMPrivateX11::_RegisterGHMPrivateX11() :
    regUnregRc(0),
    regUnregKc(KeyCombination())
{}

_RegisterGHMPrivateX11::~_RegisterGHMPrivateX11() { end(); }

int _RegisterGHMPrivateX11::doBeforeThreadRun()
{
    eventFd = eventfd(0, 0);
    if (eventFd == -1)
        return errno;
    return RC_SUCCESS;
}

int _RegisterGHMPrivateX11::doBeforeThreadEnd()
{
    EventType et = ET_EXIT;
    auto wrsize = write(eventFd, &et, 8);
    if (wrsize != 8)
        return errno;
    return RC_SUCCESS;
}

void _RegisterGHMPrivateX11::work()
{
    Display* display = NULL;
    {
        ErrorHandler eh;
        display = XOpenDisplay(NULL);
        if (eh.ec != RC_SUCCESS)
        {
            setRunFail(eh.ec);
            return;
        }
    }

    int x11Fd = ConnectionNumber(display);
    pollfd fds[2];
    fds[0].fd = x11Fd;
    fds[0].events = POLLIN;
    fds[1].fd = eventFd;
    fds[1].events = POLLIN;

    setRunSuccess();
    KeyCombination prevKc;
    KeyCombination currKc;
    XEvent event = {0};
    while (true)
    {
        auto ret = poll(fds, 2, -1);
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

        // Get the My Event.
        if (fds[1].revents & POLLIN)
        {
            EventType et;
            auto rdsize = read(eventFd, &et, 8);
            if (rdsize != 8)
                continue;

            if (et == ET_EXIT)
            {
                break;
            }
            else if (et == ET_REGISTER)
            {
                regUnregRc = nativeRegisterHotkey(display);
                cvRegUnregRc.notify_one();
            }
            else if (et == ET_UNREGISTER)
            {
                regUnregRc = nativeUnregisterHotkey(display);
                cvRegUnregRc.notify_one();
            }
        }
    }

    XCloseDisplay(display);
    close(eventFd);
}

int _RegisterGHMPrivateX11::registerHotkey(const KeyCombination& kc, bool autoRepeat)
{
    regUnregRc = -1;
    regUnregKc = kc;
    EventType et = ET_REGISTER;
    auto wrsize = write(eventFd, &et, 8);
    if (wrsize != 8)
        return errno;

    std::mutex dummyLock;
    std::unique_lock<std::mutex> lock(dummyLock);
    cvRegUnregRc.wait(lock, [this]() { return (rc != -1); });
    return regUnregRc;
}

int _RegisterGHMPrivateX11::unregisterHotkey(const KeyCombination& kc)
{
    regUnregRc = -1;
    regUnregKc = kc;
    EventType et = ET_UNREGISTER;
    auto wrsize = write(eventFd, &et, 8);
    if (wrsize != 8)
        return errno;

    std::mutex dummyLock;
    std::unique_lock<std::mutex> lock(dummyLock);
    cvRegUnregRc.wait(lock, [this]() { return (rc != -1); });
    return regUnregRc;
}

void _RegisterGHMPrivateX11::invoke(const KeyCombination& prevKc, const KeyCombination& currKc)
{
    auto pair = getPairValue(currKc);
    auto& autoRepeat = pair.first;
    auto& fn = pair.second;
    bool shouldInvoke = fn && (currKc != prevKc || autoRepeat);
    if (shouldInvoke)
        fn();
}

int _RegisterGHMPrivateX11::nativeRegisterHotkey(Display* display, const KeyCombination& kc)
{
    ErrorHandler eh;

    auto keysym = x11Keysym(kc.key());
    auto keycode = XKeysymToKeycode(display, keysym);
    keycodeTokeysym[keycode] = keysym;
    auto mod = x11Modifiers(kc.modifiers());
    XGrabKey(display, keycode, mod, DefaultRootWindow(display), True, GrabModeAsync, GrabModeAsync);
    XSync(display, False);

    return eh.ec;
}

int _RegisterGHMPrivateX11::nativeUnregisterHotkey(Display* display, const KeyCombination& kc)
{
    ErrorHandler eh;

    auto keysym = x11Keysym(kc.key());
    auto keycode = XKeysymToKeycode(display, keysym);
    auto mod = x11Modifiers(kc.modifiers());
    XUngrabKey(display, keycode, mod, DefaultRootWindow(display));
    XSync(display, False);

    return eh.ec;
}

KeyCombination _RegisterGHMPrivateX11::keyPressedCallback(int x11Keycode, int x11Modifiers)
{
    auto mod = getModifiersFromX11Modifiers(x11Modifiers);
    auto keysym = keycodeTokeysym[x11Keycode];
    auto key = getKeyFromX11Keysym(keysym);
    return {mod, key};
}

KeyCombination _RegisterGHMPrivateX11::keyReleasedCallback(int x11Keycode, int x11Modifiers)
{
    return KeyCombination();
}

int _RegisterGHMPrivateX11::ErrorHandler::ec = RC_SUCCESS;
XErrorHandler _RegisterGHMPrivateX11::ErrorHandler::prevXErrorHandler;

_RegisterGHMPrivateX11::ErrorHandler::ErrorHandler()
{
    ec = RC_SUCCESS;
    prevXErrorHandler = XSetErrorHandler(&ErrorHandler::handleError);
}

_RegisterGHMPrivateX11::ErrorHandler::~ErrorHandler()
{
    XSetErrorHandler(prevXErrorHandler);
}

int _RegisterGHMPrivateX11::ErrorHandler::handleError(Display* display, XErrorEvent* error)
{
    if (error->error_code != Success)
        ec = error->error_code;
    return ec;
}

} // namespace gbhk

#endif // _GLOBAL_HOTKEY_LINUX

#endif // !GLOBAL_HOTKEY_DISABLE_REGISTER
