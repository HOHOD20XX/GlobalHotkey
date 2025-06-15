#ifndef GLOBAL_HOTKEY_KEYBOARD_HOOK_PRIVATE_LINUX_HPP
#define GLOBAL_HOTKEY_KEYBOARD_HOOK_PRIVATE_LINUX_HPP

#ifndef GLOBAL_HOTKEY_DISABLE_HOOK

#include "keyboard_hook_private.hpp"

#ifdef _GLOBAL_HOTKEY_LINUX

#include <condition_variable>   // condition_variable
#include <thread>               // thread
#include <vector>               // vector

#include <libevdev/libevdev.h>

namespace gbhk
{

namespace kbhook
{

class _KeyboardHookPrivateLinux final : public _KeyboardHookPrivate
{
public:
    _KeyboardHookPrivateLinux();
    ~_KeyboardHookPrivateLinux();

    int start() override;
    int end() override;

private:
    void handleEvent_(int nativeKey, int state) const;
    void work_();

    std::condition_variable cvIsRunning_;
    std::atomic<bool> shouldClose_;
    std::thread workThread_;

    std::vector<std::pair<int, libevdev*>> devices_;
};

} // namespace kbhook

} // namespace gbhk

#endif // _GLOBAL_HOTKEY_LINUX

#endif // !GLOBAL_HOTKEY_DISABLE_HOOK

#endif // !GLOBAL_HOTKEY_KEYBOARD_HOOK_PRIVATE_LINUX_HPP
