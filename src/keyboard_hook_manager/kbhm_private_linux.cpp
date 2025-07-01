#ifndef GLOBAL_HOTKEY_DISABLE_HOOK

#include "kbhm_private_linux.hpp"

#ifdef _GLOBAL_HOTKEY_LINUX

#include <dirent.h>         // dirent
#include <fcntl.h>          // fcntl, open
#include <linux/input.h>    // input_event
#include <sys/eventfd.h>    // eventfd
#include <sys/inotify.h>    // inotify
#include <sys/ioctl.h>      // ioctl
#include <sys/stat.h>       // stat
#include <unistd.h>         // read, write, close

#include <global_hotkey/return_code.hpp>

namespace gbhk
{

namespace kbhook
{

constexpr const char* EVDEV_DIR = "/dev/input/";
constexpr int KEY_STATE_RELEASED = 0;
constexpr int KEY_STATE_PRESSED = 1;
constexpr int KEY_STATE_HELD = 2;

/// @return Return true if the specified path is exists and it is a character device else return false.
static bool isCharacterDevice(const std::string& filename)
{
    struct stat st;
    return (stat(filename.c_str(), &st) != -1 && S_ISCHR(st.st_mode));
}

/// @return Return true if the specified input device has `EV_KEY` event and has't `EV_REL` and `EV_ABS` event
/// else return false.
static bool isKeyboardDevice(int fd)
{
    unsigned int evbit = 0;
    if (ioctl(fd, EVIOCGBIT(0, sizeof(evbit)), &evbit) == -1)
        return false;
    bool hasKeyEvent = (evbit & (1 << EV_KEY)) != 0;
    bool hastRelEvent = (evbit & (1 << EV_REL)) == 0;
    bool hastAbsEvent = (evbit & (1 << EV_ABS)) == 0;
    return hasKeyEvent && hastRelEvent && hastAbsEvent;
}

_KBHMPrivateLinux::_KBHMPrivateLinux()
{
    fds.reserve(10 + 2);
}

_KBHMPrivateLinux::~_KBHMPrivateLinux() { end(); }

int _KBHMPrivateLinux::doBeforeThreadRun()
{
    // Set the `eventFd` to control the end of worker thread.
    eventFd = eventfd(0, EFD_CLOEXEC | EFD_NONBLOCK);
    if (eventFd == -1)
        return errno;

    // Set the `notifyFd` to watch the change of input devices.
    notifyFd = inotify_init();
    if (notifyFd == -1)
        return errno;
    int wd = inotify_add_watch(notifyFd, EVDEV_DIR, IN_CREATE | IN_DELETE);
    if (wd == -1)
        return errno;

    fds.clear();
    fds.emplace_back(pollfd{eventFd, POLLIN});
    fds.emplace_back(pollfd{notifyFd, POLLIN});

    // Traverse all input devices to obtain the FDs of input devices has `EV_KEY` event.
    evdevFdSize = 0;
    DIR* dir = opendir(EVDEV_DIR);
    if (dir == NULL)
    {
        // Rollback.
        close(eventFd);
        close(notifyFd);
        return errno;
    }
    dirent* ent = readdir(dir);
    while (ent != NULL)
    {
        std::string filename = std::string(EVDEV_DIR) + ent->d_name;
        if (isCharacterDevice(filename))
        {
            int evdevFd = open(filename.c_str(), O_RDONLY | O_NONBLOCK);
            if (evdevFd == -1)
                continue;
            if (isKeyboardDevice(evdevFd))
            {
                fds.emplace_back(pollfd{evdevFd, POLLIN});
                evdevFdSize++;
            }
            else
            {
                close(evdevFd);
            }
        }
        ent = readdir(dir);
    }
    closedir(dir);

    return RC_SUCCESS;
}

int _KBHMPrivateLinux::doBeforeThreadEnd()
{
    int64_t ev = 1;
    auto wrsize = write(eventFd, &ev, 8);
    if (wrsize != 8)
        return errno;
    return RC_SUCCESS;
}

void _KBHMPrivateLinux::work()
{
    setRunSuccess();
    input_event inputEv = {0};
    while (true)
    {
        int ret = poll(fds.data(), fds.size(), -1);
        if (ret == -1)
            continue;

        // Exit event was detected.
        if (fds[0].revents & POLLIN)
        {
            int64_t ev;
            auto rdsize = read(eventFd, &ev, 8);
            if (rdsize == 8)
                break;
        }

        // The input devices has changed.
        if (fds[1].revents & POLLIN)
        {
            // TODO: Handle the change of input devices.
        }

        // Is input devices has event?
        for (size_t i = evdevFdSize + 1; i >= 2; --i)
        {
            if (fds[i].revents & POLLIN)
            {
                auto rdsize = read(fds[i].fd, &inputEv, sizeof(input_event));
                if (rdsize != sizeof(input_event))
                    continue;
                if (inputEv.type == EV_KEY)
                {
                    int nativeKey = inputEv.code;
                    int keyState = inputEv.value;
                    handleKeyEvent(nativeKey, keyState);
                }
            }
        }
    }

    for (const auto& fd : fds)
        close(fd.fd);
}

void _KBHMPrivateLinux::handleKeyEvent(int nativeKey, int keyState)
{
    KeyState state = KS_NONE;
    if (keyState == KEY_STATE_PRESSED || keyState == KEY_STATE_HELD)
    {
        auto keyPressedCallback = getKeyPressedCallback();
        if (keyPressedCallback)
            keyPressedCallback(nativeKey);
        state = KS_PRESSED;
    }
    else if (keyState == KEY_STATE_RELEASED)
    {
        auto keyReleasedCallback = getKeyReleasedCallback();
        if (keyReleasedCallback)
            keyReleasedCallback(nativeKey);
        state = KS_RELEASED;
    }

    auto fn = getKeyListenerCallback(nativeKey, state);
    if (fn)
        fn();
}

} // namespace kbhook

} // namespace gbhk

#endif // _GLOBAL_HOTKEY_LINUX

#endif // GLOBAL_HOTKEY_DISABLE_HOOK
