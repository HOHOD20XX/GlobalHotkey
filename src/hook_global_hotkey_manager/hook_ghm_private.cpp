#ifndef GLOBAL_HOTKEY_DISABLE_HOOK

#include "hook_ghm_private.hpp"

#include <global_hotkey/return_code.hpp>

#include "../key/key_private.hpp"

namespace gbhk
{

enum EventType
{
    ET_EXIT         = 1,
    ET_KEY_PRESSED  = 2,
    ET_KEY_RELEASED = 3
};

struct Event
{
    EventType type;
    int32_t data;
};

static std::mutex mtx;
static std::condition_variable cvHasEvent;
static std::queue<Event> eventQueue;

// Block until has event.
static Event takeEvent();
static void pushEvent(const Event& event);
static void clearEventQueue();
static void keyPressedCallback(int nativeKey);
static void keyReleasedCallback(int nativeKey);

_HookGHMPrivate::_HookGHMPrivate() :
    kbhm(kbhook::KeyboardHookManager::getInstance())
{}

_HookGHMPrivate::~_HookGHMPrivate() { end(); }

int _HookGHMPrivate::doBeforeThreadRun()
{
    clearEventQueue();
    pressedMod = 0;
    pressedKeys.clear();

    int rc = kbhm.run();
    if (rc != RC_SUCCESS)
        return rc;
    kbhm.setKeyPressedCallback(&keyPressedCallback);
    kbhm.setKeyReleasedCallback(&keyReleasedCallback);
    return rc;
}

int _HookGHMPrivate::doBeforeThreadEnd()
{
    pushEvent({.type = ET_EXIT});
    return kbhm.end();
}

void _HookGHMPrivate::work()
{
    setRunSuccess();
    KeyCombination prevKc;
    while (true)
    {
        Event ev = takeEvent();

        if (ev.type == ET_EXIT)
            break;

        // ev.type == ET_KEY_PRESSED || ev.type == ET_KEY_RELEASED
        Key key = ev.data;
        if (ev.type == ET_KEY_PRESSED)
        {
            pressedKeys.insert(PriorityItem<Key>(key, pressedKeys.size()));
            if (key == Key_Mod_Meta || key == Key_Mod_Meta_Left || key == Key_Mod_Meta_Right)
                pressedMod = pressedMod.add(META);
            else if (key == Key_Mod_Ctrl || key == Key_Mod_Ctrl_Left || key == Key_Mod_Ctrl_Right)
                pressedMod = pressedMod.add(CTRL);
            else if (key == Key_Mod_Alt || key == Key_Mod_Alt_Left || key == Key_Mod_Alt_Right)
                pressedMod = pressedMod.add(ALT);
            else if (key == Key_Mod_Shift || key == Key_Mod_Shift_Left || key == Key_Mod_Shift_Right)
                pressedMod = pressedMod.add(SHIFT);
        }
        // ev.type == ET_KEY_RELEASED
        else
        {
            pressedKeys.erase(PriorityItem<Key>(key, 0));
            if (key == Key_Mod_Meta || key == Key_Mod_Meta_Left || key == Key_Mod_Meta_Right)
                pressedMod = pressedMod.remove(META);
            else if (key == Key_Mod_Ctrl || key == Key_Mod_Ctrl_Left || key == Key_Mod_Ctrl_Right)
                pressedMod = pressedMod.remove(CTRL);
            else if (key == Key_Mod_Alt || key == Key_Mod_Alt_Left || key == Key_Mod_Alt_Right)
                pressedMod = pressedMod.remove(ALT);
            else if (key == Key_Mod_Shift || key == Key_Mod_Shift_Left || key == Key_Mod_Shift_Right)
                pressedMod = pressedMod.remove(SHIFT);
        }

        Key pressedKey = *pressedKeys.crbegin();
        KeyCombination currKc(pressedMod, pressedKey);
        invoke(prevKc, currKc);
        prevKc = currKc;
    }
}

int _HookGHMPrivate::registerHotkey(const KeyCombination& kc, bool autoRepeat)
{ return RC_SUCCESS; }

int _HookGHMPrivate::unregisterHotkey(const KeyCombination& kc)
{ return RC_SUCCESS; }

void _HookGHMPrivate::invoke(const KeyCombination& prevKc, const KeyCombination& currKc)
{
    auto pair = getPairValue(currKc);
    auto& autoRepeat = pair.first;
    auto& fn = pair.second;
    bool shouldInvoke = fn && (currKc != prevKc || autoRepeat);
    if (shouldInvoke)
        fn();
}

Event takeEvent()
{
    std::unique_lock<std::mutex> lock(mtx);
    cvHasEvent.wait(lock, [&]() { !eventQueue.empty(); });
    Event ev = eventQueue.front();
    eventQueue.pop();
    return ev;
}

void pushEvent(const Event& event)
{
    std::lock_guard<std::mutex> lock(mtx);
    eventQueue.push(event);
    cvHasEvent.notify_one();
}

void clearEventQueue()
{
    std::lock_guard<std::mutex> lock(mtx);
    while (!eventQueue.empty())
        eventQueue.pop();
}

void keyPressedCallback(int nativeKey)
{
    auto key = getKeyFromNativeKey(nativeKey);
    pushEvent({.type = ET_KEY_PRESSED, .data = key});
}

void keyReleasedCallback(int nativeKey)
{
    auto key = getKeyFromNativeKey(nativeKey);
    pushEvent({.type = ET_KEY_RELEASED, .data = key});
}

} // namespace gbhk

#endif // !GLOBAL_HOTKEY_DISABLE_HOOK
