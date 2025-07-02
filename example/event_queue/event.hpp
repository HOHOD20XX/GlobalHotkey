#pragma once

#include <cstdint>

enum EventType
{
    ADD_HOTKEY,
    REMOVE_HOTKEY,
    REPLACE_HOTKEY,
    EASY_WORK,
    HEAVY_WORK,
    SET_HEAVY_WORK_TIME
};

struct Event
{
    EventType type;
    int64_t data1;
    void* data2;
};
