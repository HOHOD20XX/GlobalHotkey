#ifndef GLOBAL_HOTKEY_DISABLE_HOOK

#include "keyboard_hook_private_linux.hpp"

#ifdef _GLOBAL_HOTKEY_LINUX

#include <fcntl.h>      // open
#include <sys/stat.h>   // stat
#include <unistd.h>     // getuid, close

#include <global_hotkey/utility.hpp>

namespace gbhk
{

namespace kbhook
{

constexpr const char* EVDEV_FILE_PREFIX = "/dev/input/event";
constexpr int KEY_RELEASED = 0;
constexpr int KEY_PRESSED = 1;
constexpr int KEY_HELD = 2;

static bool isRootPermission()
{
    return ::getuid() == 0;
}

static bool isFileExists(const std::string& fileName)
{
    struct stat s;
    return ::stat(fileName.c_str(), &s) == 0;
}

/// @return A file descripitor.
static int getLibevdevFromFile(const std::string& fileName, libevdev** dev)
{
    int fd = ::open(fileName.c_str(), O_RDONLY | O_NONBLOCK);
    if (fd >= 0)
    {
        int rc = ::libevdev_new_from_fd(fd, dev);
        if (rc < 0)
            ::close(fd);
    }
    return fd;
}

_KeyboardHookPrivateLinux::_KeyboardHookPrivateLinux()
    : shouldClose_(false)
{}

_KeyboardHookPrivateLinux::~_KeyboardHookPrivateLinux()
{
    end();
}

int _KeyboardHookPrivateLinux::start()
{
    if (isRunning_)             return RC_SUCCESS;
    if (!isRootPermission())    return RC_PERMISSION_DENIED;

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
                    devices_.emplace_back(fd, dev);
            }
            evdevIndex++;
        }
        else
        {
            break;
        }
    }

    isRunning_ = true;
    workThread_ = std::thread([this]() {
        work_();
        isRunning_ = false;
        cvIsRunning_.notify_all();
    });
    workThread_.detach();

    return RC_SUCCESS;
}

int _KeyboardHookPrivateLinux::end()
{
    if (!isRunning_)
        return RC_SUCCESS;

    shouldClose_ = true;
    std::unique_lock<std::mutex> lock(mtx_);
    cvIsRunning_.wait(lock, [this]() { return !isRunning_; });
    lock_.unlock();
    shouldClose_ = false;

    for (auto& device : devices_)
    {
        int fd = device.first;
        libevdev* dev = device.second;
        ::close(fd);
        ::libevdev_free(dev);
    }
    devices_.clear();

    _KeyboardHookPrivate::resetStaticMember_();

    return RC_SUCCESS;
}

void _KeyboardHookPrivateLinux::handleEvent_(int nativeKey, int state) const
{
    std::lock_guard<std::mutex> lock(mtx_);

    int ks = 0;
    if (state == KEY_PRESSED)
    {
        if (keyPressedCallback_)
            keyPressedCallback_(nativeKey);
        ks = KS_PRESSED;
    }
    else if (state == KEY_RELEASED)
    {
        if (keyReleasedCallback_)
            keyReleasedCallback_(nativeKey);
        ks = KS_RELEASED;
    }

    Combine combine(nativeKey, static_cast<KeyState>(ks));
    if (fns_.find(combine) != fns_.end())
    {
        auto& fn = fns_[combine];
        if (fn)
            fn();
    }
}

void _KeyboardHookPrivateLinux::work_()
{
    while (!shouldClose_)
    {
        for (auto& device : devices_)
        {
            libevdev* dev = device.second;
            input_event ev;
            if (libevdev_next_event(dev, LIBEVDEV_READ_FLAG_NORMAL, &ev) == 0)
            {
                if (ev.type == EV_KEY)
                {
                    int keyCode = ev.code;
                    int keyState = ev.value;
                    handleEvent_(keyCode, keyState);
                }
            }
        }
    }
}

} // namespace kbhook

} // namespace gbhk

#endif // _GLOBAL_HOTKEY_LINUX

#endif // GLOBAL_HOTKEY_DISABLE_HOOK
