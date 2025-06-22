#ifndef GLOBAL_HOTKEY_KBHM_PRIVATE_LINUX_HPP
#define GLOBAL_HOTKEY_KBHM_PRIVATE_LINUX_HPP

#ifndef GLOBAL_HOTKEY_DISABLE_HOOK

#include "kbhm_private.hpp"

#ifdef _GLOBAL_HOTKEY_LINUX

#include <vector>   // vector

#include <libevdev/libevdev.h>

namespace gbhk
{

namespace kbhook
{

class _KBHMPrivateLinux final : public _KBHMPrivate
{
public:
    _KBHMPrivateLinux();
    ~_KBHMPrivateLinux();

protected:
    int specializedStart() override;
    int specializedEnd() override;

    void eachCycleDo() override;

private:
    void handleKeyEvent(int nativeKey, int state);

    input_event event = {0};
    std::vector<std::pair<int, libevdev*>> devices;
};

} // namespace kbhook

} // namespace gbhk

#endif // _GLOBAL_HOTKEY_LINUX

#endif // !GLOBAL_HOTKEY_DISABLE_HOOK

#endif // !GLOBAL_HOTKEY_KBHM_PRIVATE_LINUX_HPP
