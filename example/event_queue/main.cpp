#include <iostream>
#include <string>

#include "const_value.hpp"
#include "event_queue.hpp"

static EventQueue mainEventQueue;

int main()
{
    GHM.run();
    GHM.add()

    while (true)
    {
        Event ev = mainEventQueue.takeEvent();

        switch (ev.type)
        {

        }
    }

    return 0;
}
