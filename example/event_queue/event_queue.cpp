#include <cstddef>
#include <atomic>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <string>

#ifdef GLOBAL_HOTKEY_EXAMPLE_USE_HOOK
    #define GHM gbhk::HookGlobalHotkeyManager::getInstance()
#else
    #define GHM gbhk::RegisterGlobalHotkeyManager::getInstance()
#endif

enum EventType
{
    ADD_HOTKEY,
    REMOVE_HOTKEY,
    REPLACE_HOTKEY,
    SET_CALLBACK_PRINT,
    A_EASY_WORK,
    A_HEAVY_WORK
};

struct Event
{
    EventType type;
    int64_t data1;
    void* data2;
};

static bool hasEvent();
static void pushEvent(const Event& event);
static Event takeEvent();
static void clearEvent();

int main()
{
    return 0;
}

bool hasEvent()
{

}

void pushEvent(const Event &event)
{

}

Event takeEvent()
{
    return Event();
}

void clearEvent()
{

}
