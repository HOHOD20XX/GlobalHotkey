#ifndef GLOBAL_HOTKEY_KBHM_PRIVATE_MAC_HPP
#define GLOBAL_HOTKEY_KBHM_PRIVATE_MAC_HPP

#ifndef GLOBAL_HOTKEY_DISABLE_HOOK

#include "kbhm_private.hpp"

#ifdef _GLOBAL_HOTKEY_MAC

#include <Carbon/Carbon.h>
#include <CoreFoundation/CoreFoundation.h>
#include <CoreGraphics/CGEvent.h>

namespace gbhk
{

namespace kbhook
{

class _KBHMPrivateMac final : public _KBHMPrivate
{
public:
    _KBHMPrivateMac();
    ~_KBHMPrivateMac();

protected:
    int doBeforeThreadEnd() override;
    void work() override;

private:
    static CGEventRef keyboardTapCallback(
        CGEventTapProxy proxy,
        CGEventType type,
        CGEventRef event,
        void* data);

    CFRunLoopRef runLoop = NULL;
};

} // namespace kbhook

} // namespace gbhk

#endif // _GLOBAL_HOTKEY_MAC

#endif // !GLOBAL_HOTKEY_DISABLE_HOOK

#endif // !GLOBAL_HOTKEY_KBHM_PRIVATE_MAC_HPP
