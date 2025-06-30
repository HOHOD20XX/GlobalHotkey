#ifndef GLOBAL_HOTKEY_KBHM_PRIVATE_LINUX_HPP
#define GLOBAL_HOTKEY_KBHM_PRIVATE_LINUX_HPP

#ifndef GLOBAL_HOTKEY_DISABLE_HOOK

#include "kbhm_private.hpp"

#ifdef _GLOBAL_HOTKEY_LINUX

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
    int doBeforeThreadRun() override;
    int doBeforeThreadEnd() override;
    void work() override;

private:
    void handleKeyEvent(int nativeKey, int keyState);

    int eventFd = 0;
    int notifyFd = 0;
};

} // namespace kbhook

} // namespace gbhk

#endif // _GLOBAL_HOTKEY_LINUX

#endif // !GLOBAL_HOTKEY_DISABLE_HOOK

#endif // !GLOBAL_HOTKEY_KBHM_PRIVATE_LINUX_HPP
