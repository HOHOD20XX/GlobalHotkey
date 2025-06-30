#ifndef GLOBAL_HOTKEY_DISABLE_REGISTER

#include "register_ghm_private_mac.hpp"

#ifdef _GLOBAL_HOTKEY_MAC

#include <global_hotkey/return_code.hpp>

#include "../key/key_private.hpp"

namespace gbhk
{

enum EventType
{
    ET_REGISTER = 1,
    ET_UNREGISTER
};

std::condition_variable _RegisterGHMPrivateMac::cvRegUnregRc;
std::atomic<int> _RegisterGHMPrivateMac::regUnregRc(0);
std::atomic<KeyCombination> _RegisterGHMPrivateMac::regUnregKc;
std::atomic<bool> _RegisterGHMPrivateMac::regUnregAutoRepeat(false);
std::unordered_map<KeyCombination, EventHotKeyRef> _RegisterGHMPrivateMac::kcToHotkeyRef;

_RegisterGHMPrivateMac::_RegisterGHMPrivateMac() = default;

_RegisterGHMPrivateMac::~_RegisterGHMPrivateMac() { end(); }

int _RegisterGHMPrivateMac::doBeforeThreadRun()
{
    sourceContext = {
        .version = 0,
        .info = NULL,
        .retain = NULL,
        .release = NULL,
        .copyDescription = NULL,
        .equal = NULL,
        .hash = NULL,
        .schedule = NULL,
        .cancel = NULL,
        .perform = &_RegisterGHMPrivateMac::runLoopSourceCallback,
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
    source = CFRunLoopSourceCreate(NULL, 0, &sourceContext);
    if (source == NULL)
    {
        // TODO: Need error code.
        setRunFail(-1);
        return;
    }

    runLoop = CFRunLoopGetCurrent();
    if (runLoop == NULL)
    {
        // TODO: Need error code.
        setRunFail(-1);
        return;
    }

    CFRunLoopAddSource(runLoop, source, kCFRunLoopDefaultMode);

    EventTypeSpec eventTypes[2] = {0};
    eventTypes[0].eventClass = kEventClassKeyboard;
    eventTypes[0].eventKind = kEventHotKeyPressed;
    eventTypes[1].eventClass = kEventClassKeyboard;
    eventTypes[1].eventKind = kEventHotKeyReleased;
    auto status = InstallApplicationEventHandler(&hotkeyEventHandler, 2, eventTypes, NULL, NULL);
    if (status != noErr)
    {
        setRunFail(status);
        return;
    }

    setRunSuccess();
    CFRunLoopRun();

    CFRelease(source);
}

int _RegisterGHMPrivateMac::registerHotkey(const KeyCombination& kc, bool autoRepeat)
{
    regUnregRc = -1;
    regUnregKc = kc;
    EventType et = ET_REGISTER;
    sourceContext.info = &et;
    printf("1\n");
    CFRunLoopSourceSignal(source);
    CFRunLoopWakeUp(runLoop);
    printf("2\n");
    std::mutex dummyMtx;
    std::unique_lock<std::mutex> lock(dummyMtx);
    cvRegUnregRc.wait(lock, [this]() { return regUnregRc != -1; });
    printf("3\n");
    return regUnregRc;
}

int _RegisterGHMPrivateMac::unregisterHotkey(const KeyCombination& kc)
{
    regUnregRc = -1;
    regUnregKc = kc;
    EventType et = ET_UNREGISTER;
    sourceContext.info = &et;
    CFRunLoopSourceSignal(source);
    CFRunLoopWakeUp(runLoop);

    std::mutex dummyMtx;
    std::unique_lock<std::mutex> lock(dummyMtx);
    cvRegUnregRc.wait(lock, [this]() { return regUnregRc != -1; });
    return regUnregRc;
}

void _RegisterGHMPrivateMac::runLoopSourceCallback(void* data)
{
    printf("ET Address: %llu\n", (unsigned long long)data);
    printf("11\n");
    if (data)
    {
        EventType et = *(EventType*) (data);
        printf("et: %d\n", et);
        printf("12\n");
        if (et == ET_REGISTER)
        {
            printf("13\n");
            regUnregRc = nativeRegisterHotkey();
            cvRegUnregRc.notify_one();
            printf("14\n");
        }
        else if (et == ET_UNREGISTER)
        {
            regUnregRc = nativeUnregisterHotkey();
            cvRegUnregRc.notify_one();
        }
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
    EventHotKeyID hotkeyId = {
        .signature = OSType(regUnregKc.load().modifiers()),
        .id = UInt32(regUnregKc.load().key())
    };
    EventHotKeyRef ref;
    auto status = RegisterEventHotKey(
        hotkeyId.id,
        hotkeyId.signature,
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
