#ifndef GLOBAL_HOTKEY_DISABLE_HOOK

#include "kbhm_private_linux.hpp"

#ifdef _GLOBAL_HOTKEY_LINUX

#include <cstdint>
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

constexpr size_t BUF_ITEM_COUNT = 32;
constexpr size_t INOTIFY_EV_BUF_SIZE = BUF_ITEM_COUNT * (sizeof(inotify_event) + NAME_MAX + 1);
constexpr size_t INPUT_EV_BUF_SIZE = BUF_ITEM_COUNT * sizeof(input_event);

static char inotifyEventBuf[INOTIFY_EV_BUF_SIZE] = {0};
static char inputEventBuf[INPUT_EV_BUF_SIZE] = {0};

/// @return Return true if the specified path is exists and it is a character device else return false.
static bool isCharacterDevice(const std::string& filename)
{
    struct stat st;
    return stat(filename.c_str(), &st) != -1 && S_ISCHR(st.st_mode);
}

/// @return Return true if the specified input device has `EV_KEY` event and has't `EV_REL` and `EV_ABS` event
/// else return false.
static bool isKeyboardDevice(int fd)
{
    unsigned int evbit = 0;
    if (ioctl(fd, EVIOCGBIT(0, sizeof(unsigned int)), &evbit) == -1)
        return false;
    bool hasKeyEvent = (evbit & (1 << EV_KEY)) != 0;
    bool hastRelEvent = (evbit & (1 << EV_REL)) == 0;
    bool hastAbsEvent = (evbit & (1 << EV_ABS)) == 0;
    return hasKeyEvent && hastRelEvent && hastAbsEvent;
}

_KBHMPrivateLinux::_KBHMPrivateLinux()
{
    pollFds.reserve(10 + otherFdCount);
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

    evdevFdCount = 0;
    pollFds.clear();
    evdevFdNameFdMap.clear();

    pollFds.emplace_back(pollfd{eventFd, POLLIN});
    pollFds.emplace_back(pollfd{notifyFd, POLLIN});

    // Traverse all input devices to obtain the FDs of input devices has `EV_KEY` event.
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
        addEvdevFd(ent->d_name);
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
    while (true)
    {
        int ret = poll(pollFds.data(), pollFds.size(), -1);
        if (ret == -1)
            continue;

        // Exit event was detected.
        if (pollFds[0].revents & POLLIN)
        {
            int64_t ev;
            auto rdsize = read(eventFd, &ev, 8);
            if (rdsize == 8)
                break;
        }

        // The input devices has changed.
        if (pollFds[1].revents & POLLIN)
        {
            auto rdsize = read(notifyFd, inotifyEventBuf, INOTIFY_EV_BUF_SIZE);
            if (rdsize <= 0)
                continue;

            char* p = inotifyEventBuf;
            while (p < inotifyEventBuf + rdsize)
            {
                inotify_event* ev = (inotify_event*) p;

                if (ev->mask == IN_Q_OVERFLOW)
                {
                    // Pass.
                    continue;
                }

                if (ev->mask == IN_CREATE && ev->len > 0)
                    addEvdevFd(ev->name);
                else if (ev->mask == IN_DELETE && ev->len > 0)
                    removeEvdevFd(ev->name);

                p += sizeof(inotify_event) + ev->len;
            }
        }

        // Is input devices has event?
        for (size_t i = evdevFdCount + 1; i >= otherFdCount; --i)
        {
            if (pollFds[i].revents & POLLIN)
            {
                auto rdsize = read(pollFds[i].fd, inputEventBuf, INPUT_EV_BUF_SIZE);
                if (rdsize <= 0)
                    continue;

                char* p = inputEventBuf;
                while (p < inputEventBuf + rdsize)
                {
                    input_event* ev = (input_event*) p;
                    if (ev->type == EV_KEY)
                    {
                        int nativeKey = ev->code;
                        int keyState = ev->value;
                        handleKeyEvent(nativeKey, keyState);
                    }
                    p += sizeof(input_event);
                }
            }
        }
    }

    for (const auto& fd : pollFds)
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

void _KBHMPrivateLinux::addEvdevFd(const std::string& name)
{
    if (name.empty())
        return;

    std::string filename = EVDEV_DIR + name;
    if (isCharacterDevice(filename))
    {
        int evdevFd = open(filename.c_str(), O_RDONLY | O_NONBLOCK);
        if (evdevFd == -1)
            return;

        if (isKeyboardDevice(evdevFd))
        {
            pollFds.emplace_back(pollfd{evdevFd, POLLIN});
            evdevFdNameFdMap[name] = evdevFd;
            evdevFdCount++;
        }
        else
        {
            close(evdevFd);
        }
    }
}

void _KBHMPrivateLinux::removeEvdevFd(const std::string& name)
{
    if (evdevFdNameFdMap.find(name) == evdevFdNameFdMap.end())
        return;

    int evdevFd = evdevFdNameFdMap[name];
    for (auto it = pollFds.begin(); it != pollFds.end(); ++it)
    {
        if (it->fd == evdevFd)
        {
            close(it->fd);
            pollFds.erase(it);
            evdevFdNameFdMap.erase(name);
            evdevFdCount--;
            return;
        }
    }
}

} // namespace kbhook

} // namespace gbhk

#endif // _GLOBAL_HOTKEY_LINUX

#endif // GLOBAL_HOTKEY_DISABLE_HOOK
