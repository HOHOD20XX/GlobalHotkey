#ifndef GLOBAL_HOTKEY_DISABLE_HOOK

#include "kbhm_private_mac.hpp"

#ifdef _GLOBAL_HOTKEY_MAC

#include <array>    // array

#include <global_hotkey/return_code.hpp>

namespace gbhk
{

namespace kbhook
{

// TODO: Use error code instead of the current placeholder return codes (-1).

constexpr int MODKC_FIRST = kVK_RightCommand;
constexpr int MODKC_LAST = kVK_Function;
constexpr int MODKC_COUNT = MODKC_LAST - MODKC_FIRST + 1;

#define GMSINDEX(keycode) ((keycode) - MODKC_FIRST)

// The modifier key states.
// True if the modifier key is pressed else false.
static std::array<bool, MODKC_COUNT> modifierStates;

#define GMS(keycode) modifierStates[GMSINDEX(keycode)]

_KBHMPrivateMac::_KBHMPrivateMac() = default;

_KBHMPrivateMac::~_KBHMPrivateMac() { end(); }

int _KBHMPrivateMac::doBeforeThreadEnd()
{
    CFRunLoopStop(runLoop);
    return RC_SUCCESS;
}

void _KBHMPrivateMac::work()
{
    for (auto& modifierState : modifierStates)
        modifierState = false;

    CGEventMask eventMask =
        CGEventMaskBit(kCGEventKeyDown) |
        CGEventMaskBit(kCGEventKeyUp) |
        CGEventMaskBit(kCGEventFlagsChanged);
    CFMachPortRef eventTap = CGEventTapCreate(
        kCGHIDEventTap,
        kCGHeadInsertEventTap,
        kCGEventTapOptionDefault,
        eventMask,
        &keyboardTapCallback,
        NULL
    );
    if (!eventTap)
    {
        setRunFail(-1);
        return;
    }

    runLoop = CFRunLoopGetCurrent();
    if (!runLoop)
    {
        setRunFail(-1);
        return;
    }
    CFRunLoopSourceRef runLoopSource =
        CFMachPortCreateRunLoopSource(kCFAllocatorDefault, eventTap, 0);
    if (!runLoopSource)
    {
        setRunFail(-1);
        return;
    }
    CFRunLoopAddSource(runLoop, runLoopSource, kCFRunLoopCommonModes);
    CGEventTapEnable(eventTap, true);

    setRunSuccess();
    CFRunLoopRun();

    CFRelease(eventTap);
    CFRelease(runLoopSource);
}

CGEventRef _KBHMPrivateMac::keyboardTapCallback(
    CGEventTapProxy proxy,
    CGEventType type,
    CGEventRef event,
    void* data)
{
    CGKeyCode keyCode =
        CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);
    KeyState state = KS_NONE;
    if (type == kCGEventKeyDown)
    {
        auto keyPressedCallback = getKeyPressedCallback();
        if (keyPressedCallback)
            keyPressedCallback(keyCode);
        state = KS_PRESSED;
    }
    else if (type == kCGEventKeyUp)
    {
        auto keyReleasedCallback = getKeyReleasedCallback();
        if (keyReleasedCallback)
            keyReleasedCallback(keyCode);
        state = KS_RELEASED;
    }
    else if (type == kCGEventFlagsChanged)
    {
        auto& modifierState = GMS(keyCode);
        // The modifier key has been pressed.
        if (modifierState)
        {
            modifierState = false;
            auto keyReleasedCallback = getKeyReleasedCallback();
            if (keyReleasedCallback)
                keyReleasedCallback(keyCode);
            state = KS_RELEASED;
        }
        // The modifier key has been released.
        else
        {
            modifierState = true;
            auto keyPressedCallback = getKeyPressedCallback();
            if (keyPressedCallback)
                keyPressedCallback(keyCode);
            state = KS_PRESSED;
        }

        auto fn = getKeyListenerCallback(keyCode, state);
        if (fn)
            fn();
    }

    return event;
}

} // namespace kbhook

} // namespace gbhk

#endif // _GLOBAL_HOTKEY_MAC

#endif // !GLOBAL_HOTKEY_DISABLE_HOOK
