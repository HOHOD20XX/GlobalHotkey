#include <Carbon/Carbon.h>
#include <CoreFoundation/CoreFoundation.h>

static int nativeRegisterHotkey(UInt32 mod, UInt32 key)
{
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
    return status;
}

static OSStatus hotkeyEventHandler(EventHandlerCallRef handler, EventRef event, void* userData)
{
    printf("Event Class: %d, Kind: %d\n", GetEventClass(event), GetEventKind(event));
    if (GetEventClass(event) == kEventClassKeyboard)
    {
        printf("Has Event");
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

static CFRunLoopRef runLoop = NULL;

int main()
{
    runLoop = CFRunLoopGetCurrent();

    EventTypeSpec eventTypeSpecs[2];
    eventTypeSpecs[0].eventClass = kEventClassKeyboard;
    eventTypeSpecs[0].eventKind = kEventHotKeyPressed;
    eventTypeSpecs[1].eventClass = kEventClassKeyboard;
    eventTypeSpecs[1].eventKind = kEventHotKeyReleased;
    auto status = InstallApplicationEventHandler(
        &hotkeyEventHandler,
        2,
        eventTypeSpecs,
        NULL,
        NULL
    );

    UInt32 mod = 256;    // 'cmdKey'
    UInt32 key = 38;    // 'J'
    int rc = nativeRegisterHotkey(mod, key);
    printf("Register: %d\n", rc);

    CFRunLoopRun();
}
