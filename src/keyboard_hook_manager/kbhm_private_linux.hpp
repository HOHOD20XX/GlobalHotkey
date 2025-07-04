#ifndef GLOBAL_HOTKEY_KBHM_PRIVATE_LINUX_HPP
#define GLOBAL_HOTKEY_KBHM_PRIVATE_LINUX_HPP

#ifndef GLOBAL_HOTKEY_DISABLE_HOOK

#include "kbhm_private.hpp"

#ifdef _GLOBAL_HOTKEY_LINUX

#include <cstddef>  // size_t
#include <poll.h>   // poll
#include <vector>   // vector
#include <string>   // string and the specialization of hash for string.

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
    void addEvdevFd(const std::string& name);
    void removeEvdevFd(const std::string& name);

    static constexpr size_t otherFdCount = 2;

    size_t evdevFdCount = 0;
    int eventFd = 0;
    int notifyFd = 0;
    std::vector<pollfd> pollFds;
    std::unordered_map<std::string, int> evdevFdNameFdMap;
};

} // namespace kbhook

} // namespace gbhk

#endif // _GLOBAL_HOTKEY_LINUX

#endif // !GLOBAL_HOTKEY_DISABLE_HOOK

#endif // !GLOBAL_HOTKEY_KBHM_PRIVATE_LINUX_HPP
