#ifndef GLOBAL_HOTKEY_DISABLE_REGISTER

#include "register_ghm_private_mac.hpp"

#ifdef _GLOBAL_HOTKEY_MAC

#include <global_hotkey/return_code.hpp>

#include "../key/key_private.hpp"

namespace gbhk
{

enum EventType
{
    ET_REGISTER,
    ET_UNREGISTER
};

std::condition_variable _RegisterGHMPrivateMac::cvRegUnregRc;
std::atomic<int> _RegisterGHMPrivateMac::regUnregRc(0);
std::atomic<int> _RegisterGHMPrivateMac::eventType(0);
std::atomic<KeyCombination> _RegisterGHMPrivateMac::regUnregKc;
std::unordered_map<KeyCombination, EventHotKeyRef> _RegisterGHMPrivateMac::kcToHotkeyRef;

_RegisterGHMPrivateMac::_RegisterGHMPrivateMac() = default;

_RegisterGHMPrivateMac::~_RegisterGHMPrivateMac() { end(); }

int _RegisterGHMPrivateMac::doBeforeThreadRun()
{
    sourceContext = {
        .version = 0,
        .info = &source,
        .retain = NULL,
        .release = NULL,
        .copyDescription = NULL,
        .equal = NULL,
        .hash = NULL,
        .schedule = NULL,
        .cancel = NULL,
        .perform = &_RegisterGHMPrivateMac::runLoopSourceCallback
    };
    return RC_SUCCESS;
}

int _RegisterGHMPrivateMac::doBeforeThreadEnd()
{
    CFRunLoopStop(runLoop);
    return RC_SUCCESS;
}

void _RegisterGHMPrivateMac::work()
{
    runLoop = CFRunLoopGetCurrent();
    if (runLoop == NULL)
    {
        // TODO: Need error code.
        setRunFail(-1);
        return;
    }

    EventTypeSpec eventTypeSpecs[2];
    eventTypeSpecs[0].eventClass = kEventClassKeyboard;
    eventTypeSpecs[0].eventKind = kEventHotKeyPressed;
    eventTypeSpecs[1].eventClass = kEventClassKeyboard;
    eventTypeSpecs[1].eventKind = kEventHotKeyReleased;
    auto status = InstallApplicationEventHandler(
        &_RegisterGHMPrivateMac::hotkeyEventHandler,
        2,
        eventTypeSpecs,
        NULL, NULL
    );
    if (status != noErr)
    {
        setRunFail(status);
        return;
    }

    source = CFRunLoopSourceCreate(NULL, 0, &sourceContext);
    if (source == NULL)
    {
        // TODO: Need error code.
        setRunFail(-1);
        return;
    }
    CFRunLoopAddSource(runLoop, source, kCFRunLoopDefaultMode);

    setRunSuccess();
    CFRunLoopRun();

    CFRelease(source);
}

int _RegisterGHMPrivateMac::registerHotkey(const KeyCombination& kc, bool autoRepeat)
{
    regUnregRc = -1;
    eventType = ET_REGISTER;
    regUnregKc = kc;
    CFRunLoopSourceSignal(source);
    CFRunLoopWakeUp(runLoop);

    std::mutex dummyMtx;
    std::unique_lock<std::mutex> lock(dummyMtx);
    cvRegUnregRc.wait(lock, [this]() { return regUnregRc != -1; });
    return regUnregRc;
}

int _RegisterGHMPrivateMac::unregisterHotkey(const KeyCombination& kc)
{
    regUnregRc = -1;
    eventType = ET_UNREGISTER;
    regUnregKc = kc;
    CFRunLoopSourceSignal(source);
    CFRunLoopWakeUp(runLoop);

    std::mutex dummyMtx;
    std::unique_lock<std::mutex> lock(dummyMtx);
    cvRegUnregRc.wait(lock, [this]() { return regUnregRc != -1; });
    return regUnregRc;
}

void _RegisterGHMPrivateMac::runLoopSourceCallback(void* info)
{
    CFRunLoopSourceRef* sref = (CFRunLoopSourceRef*) info;
    switch (eventType.load())
    {
        case ET_REGISTER:
            regUnregRc = nativeRegisterHotkey();
            cvRegUnregRc.notify_one();
            break;
        case ET_UNREGISTER:
            regUnregRc = nativeUnregisterHotkey();
            cvRegUnregRc.notify_one();
            break;
        default:
            break;
    }
}

OSStatus _RegisterGHMPrivateMac::hotkeyEventHandler(EventHandlerCallRef handler, EventRef event, void* userData)
{
    if (GetEventClass(event) == kEventClassKeyboard)
    {
        auto eventKind = GetEventKind(event);
        if (eventKind != kEventHotKeyPressed && eventKind != kEventHotKeyReleased)
            return noErr;

        EventHotKeyID hotkeyId;
        auto status = GetEventParameter(
            event,
            kEventParamDirectObject,
            typeEventHotKeyID,
            NULL,
            sizeof(hotkeyId),
            NULL,
            &hotkeyId
        );
        if (status != noErr)
            return status;

        auto nativeModifiers = hotkeyId.signature;
        auto nativeKey = hotkeyId.id;

        if (eventKind == kEventHotKeyPressed)
        {
            printf("KeyPressed: %d\n", nativeKey);
        }
        // eventKind == kEventHotKeyReleased
        else
        {
            printf("KeyReleased: %d\n", nativeKey);
        }
    }

    return noErr;
}

void _RegisterGHMPrivateMac::invoke() const
{

}

int _RegisterGHMPrivateMac::nativeRegisterHotkey()
{
    KeyCombination kc = regUnregKc;
    UInt32 mod = (UInt32) nativeModifiers(kc.modifiers());
    UInt32 key = (UInt32) nativeKey(kc.key());

    EventHotKeyID hotkeyId = {
        .signature = (OSType) mod,
        .id = key
    };
    EventHotKeyRef ref = 0;
    auto status = RegisterEventHotKey(
        key,
        mod,
        hotkeyId,
        GetApplicationEventTarget(),
        0,
        &ref
    );
    if (status != noErr)
        return status;
    kcToHotkeyRef[regUnregKc] = ref;
    return RC_SUCCESS;
}

int _RegisterGHMPrivateMac::nativeUnregisterHotkey()
{
    EventHotKeyRef ref = kcToHotkeyRef[regUnregKc];
    auto status = UnregisterEventHotKey(ref);
    if (status != noErr)
        return status;
    kcToHotkeyRef.erase(regUnregKc);
    return RC_SUCCESS;
}

} // namespace gbhk

#endif // _GLOBAL_HOTKEY_MAC

#endif // !GLOBAL_HOTKEY_DISABLE_REGISTER
