#ifndef GLOBAL_HOTKEY_DISABLE_HOOK

#include "kbhm_private_linux.hpp"

#ifdef _GLOBAL_HOTKEY_LINUX

#include <dirent.h>         // dirent,
#include <fcntl.h>          // fcntl
#include <poll.h>           // poll
#include <sys/eventfd.h>    // eventfd
#include <sys/ioctl.h>      // ioctl
#include <unistd.h>         // read, write, open, close

#include <global_hotkey/return_code.hpp>

namespace gbhk
{

namespace kbhook
{

constexpr const char* EVDEV_DIR = "/dev/input/";
constexpr int KEY_RELEASED = 0;
constexpr int KEY_PRESSED = 1;
constexpr int KEY_HELD = 2;

static bool isFileExists(const std::string& fileName)
{
    struct stat s;
    return stat(fileName.c_str(), &s) == 0;
}

/// @return A file descripitor.
static int getLibevdevFromFile(const std::string& fileName, libevdev** dev)
{
    int fd = open(fileName.c_str(), O_RDONLY | O_NONBLOCK);
    if (fd >= 0)
    {
        int rc = libevdev_new_from_fd(fd, dev);
        if (rc < 0)
            close(fd);
    }
    return fd;
}

_KBHMPrivateLinux::_KBHMPrivateLinux() = default;

_KBHMPrivateLinux::~_KBHMPrivateLinux() { end(); }

int _KBHMPrivateLinux::doBeforeThreadRun()
{
    DIR* dir = opendir(EVDEV_DIR);
    if (dir == NULL)
        return errno;
    dirent* ent;
    int evdevIndex = 0;
    while (true)
    {
        std::string evdevFileName = EVDEV_FILE_PREFIX + std::to_string(evdevIndex);
        if (isFileExists(evdevFileName))
        {
            libevdev* dev = nullptr;
            int fd = getLibevdevFromFile(evdevFileName, &dev);
            if (fd >= 0)
            {
                if (libevdev_has_event_type(dev, EV_KEY))
                    devices.emplace_back(fd, dev);
            }
            evdevIndex++;
        }
        else
        {
            break;
        }
    }

    return RC_SUCCESS;
}

int _KBHMPrivateLinux::doBeforeThreadEnd()
{
    for (auto& device : devices)
    {
        int fd = device.first;
        libevdev* dev = device.second;
        close(fd);
        libevdev_free(dev);
    }
    event = {0};
    devices.clear();

    return RC_SUCCESS;
}

void _KBHMPrivateLinux::work()
{
    for (auto& device : devices)
    {
        libevdev* dev = device.second;
        int rc = libevdev_next_event(dev, LIBEVDEV_READ_FLAG_NORMAL, &event);
        if (rc == LIBEVDEV_READ_STATUS_SUCCESS)
        {
            if (event.type == EV_KEY)
            {
                int keyCode = event.code;
                int keyState = event.value;
                handleKeyEvent(keyCode, keyState);
            }
        }
    }
}

void _KBHMPrivateLinux::handleKeyEvent(int nativeKey, int state)
{
    std::lock_guard<std::mutex> lock(mtx);

    int ks = 0;
    if (state == KEY_PRESSED)
    {
        if (keyPressedCallback)
            keyPressedCallback(nativeKey);
        ks = KS_PRESSED;
    }
    else if (state == KEY_RELEASED)
    {
        if (keyReleasedCallback)
            keyReleasedCallback(nativeKey);
        ks = KS_RELEASED;
    }

    Combination combine(nativeKey, static_cast<KeyState>(ks));
    if (fns.find(combine) != fns.end())
    {
        auto& fn = fns[combine];
        if (fn)
            fn();
    }
}

} // namespace kbhook

} // namespace gbhk

#endif // _GLOBAL_HOTKEY_LINUX

#endif // GLOBAL_HOTKEY_DISABLE_HOOK
