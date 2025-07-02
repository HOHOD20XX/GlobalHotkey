#include "event_handler.hpp"

#include "const_value.hpp"

void exit()
{
    int rc = GHM.end();
    if (rc == RC_SUCCESS)
        printf("Success to exit.");
    else
        printf("Failed to exit, error message: %s.", returnCodeMessage(rc).c_str());
}
