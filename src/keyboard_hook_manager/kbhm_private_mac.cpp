#ifndef GLOBAL_HOTKEY_DISABLE_HOOK

#include "kbhm_private_mac.hpp"

#ifdef _GLOBAL_HOTKEY_MAC

#include <global_hotkey/return_code.hpp>

#include <iostream>

namespace gbhk
{

namespace kbhook
{

_KBHMPrivateMac::_KBHMPrivateMac() = default;

_KBHMPrivateMac::~_KBHMPrivateMac() { end(); }

int _KBHMPrivateMac::doBeforeLoop()
{
    runLoop = CFRunLoopGetCurrent();

    CGEventMask eventMask =
        CGEventMaskBit(kCGEventKeyDown) |
        CGEventMaskBit(kCGEventKeyUp) |
        CGEventMaskBit(kCGEventFlagsChanged);
    eventTap = CGEventTapCreate(
        kCGHIDEventTap,
        kCGHeadInsertEventTap,
        kCGEventTapOptionDefault,
        eventMask,
        &keyboardCallback,
        NULL);

    // TODO
    // if (!eventTap)
    // {
    //     return RC_FAIL;
    // }

    runLoopSource = CFMachPortCreateRunLoopSource(kCFAllocatorDefault, eventTap, 0);
    CFRunLoopAddSource(runLoop, runLoopSource, kCFRunLoopCommonModes);
    CGEventTapEnable(eventTap, true);

    return RC_SUCCESS;
}

int _KBHMPrivateMac::doBeforeThreadEnd()
{
    CFRelease(eventTap);
    CFRelease(runLoopSource);
    CFRunLoopStop(runLoop);

    runLoop = NULL;
    eventTap = NULL;
    runLoopSource = NULL;

    return RC_SUCCESS;
}

void _KBHMPrivateMac::eachCycleDo()
{
    CFRunLoopRun();
}

CGEventRef _KBHMPrivateMac::keyboardCallback(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void* data)
{
    if (type == kCGEventKeyDown)
    {
        CGKeyCode keycode = CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);
        printf("Key Downed: %d\n", keycode);
    }
    else if (type == kCGEventKeyUp)
    {
        CGKeyCode keycode = CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);
        printf("Key Released: %d\n", keycode);
    }
    else if (type == kCGEventFlagsChanged)
    {
        CGKeyCode keycode = CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);
        printf("Key Flag Changed: %d\n", keycode);
    }

    return event;
}

} // namespace kbhook

} // namespace gbhk

#endif // _GLOBAL_HOTKEY_MAC

#endif // !GLOBAL_HOTKEY_DISABLE_HOOK
