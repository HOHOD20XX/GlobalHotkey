#ifndef GLOBAL_HOTKEY_REGISTER_GHM_PRIVATE_MAC_HPP
#define GLOBAL_HOTKEY_REGISTER_GHM_PRIVATE_MAC_HPP

#ifndef GLOBAL_HOTKEY_DISABLE_REGISTER

#include "register_ghm_private.hpp"

#define _GLOBAL_HOTKEY_MAC
#ifdef _GLOBAL_HOTKEY_MAC

#include <Carbon/Carbon.h>
#include <CoreFoundation/CoreFoundation.h>

namespace gbhk
{

class _RegisterGHMPrivateMac final : public _RegisterGHMPrivate
{
public:
    _RegisterGHMPrivateMac();
    ~_RegisterGHMPrivateMac();

protected:
    int doBeforeThreadRun() override;
    int doBeforeThreadEnd() override;
    void work() override;
    int registerHotkey(const KeyCombination& kc, bool autoRepeat) override;
    int unregisterHotkey(const KeyCombination& kc) override;

private:
    static void runLoopSourceCallback(void* info);
    static OSStatus hotkeyEventHandler(EventHandlerCallRef handler, EventRef event, void* userData);

    void invoke() const;
    static int nativeRegisterHotkey();
    static int nativeUnregisterHotkey();

    static std::condition_variable cvRegUnregRc;
    static std::atomic<int> regUnregRc;
    static std::atomic<int> eventType;
    static std::atomic<KeyCombination> regUnregKc;
    static std::unordered_map<KeyCombination, EventHotKeyRef> kcToHotkeyRef;

    CFRunLoopSourceContext sourceContext = {0};
    CFRunLoopSourceRef source = NULL;
    CFRunLoopRef runLoop = NULL;
};

} // namespace gbhk

#endif // _GLOBAL_HOTKEY_MAC

#endif // !GLOBAL_HOTKEY_DISABLE_REGISTER

#endif // !GLOBAL_HOTKEY_REGISTER_GHM_PRIVATE_MAC_HPP
