#ifndef GLOBAL_HOTKEY_RETURN_CODE_HPP
#define GLOBAL_HOTKEY_RETURN_CODE_HPP

#include <string>   // to_string

#include "base.hpp"

namespace gbhk
{

#ifdef _GLOBAL_HOTKEY_WIN
    constexpr int _RC_MASK = 1 << 29;
#elif defined(_GLOBAL_HOTKEY_MAC)
    constexpr int _RC_MASK = 0;
#elif defined(_GLOBAL_HOTKEY_LINUX)
    constexpr int _RC_MASK = 0;
#else
    constexpr int _RC_MASK = 0;
#endif // _GLOBAL_HOTKEY_WIN

enum ReturnCode : int
{
    RC_SUCCESS                      = 0x00 | _RC_MASK,

    RC_ARR_GHM_IN_WRONG_TIME        = 0x10 | _RC_MASK,
    RC_ARR_GHM_IN_WRONG_THREAD      = 0x11 | _RC_MASK,
    RC_END_GHM_IN_WRONG_THREAD      = 0x12 | _RC_MASK,

    RC_INVALID_VALUE                = 0x20 | _RC_MASK,
    RC_EXIST_SAME_VALUE             = 0x21 | _RC_MASK,
    RC_NO_SPECIFIED_VALUE           = 0x22 | _RC_MASK,
    RC_NO_CHANGED_VALUE             = 0x23 | _RC_MASK,

    RC_PERMISSION_DENIED            = 0x30 | _RC_MASK
};

inline String returnCodeMessage(int rc) noexcept
{
    switch (rc)
    {
        case RC_SUCCESS:
            return "Success";
        case RC_ARR_GHM_IN_WRONG_TIME:
            return "Add, remove or replace global hotkey for the Global Hotkey Manager, " \
                   "but GHM is not running yet";
        case RC_ARR_GHM_IN_WRONG_THREAD:
            return "Add, remove or replace global hotkey for the Global Hotkey Manager, " \
                   "but the caller is in the same thread as GHM's work thread";
        case RC_END_GHM_IN_WRONG_THREAD:
            return "End the Global Hotkey Manager in the same thread as GHM's work thread";
        case RC_INVALID_VALUE:
            return "Invalid value";
        case RC_EXIST_SAME_VALUE:
            return "Already exist same value";
        case RC_NO_SPECIFIED_VALUE:
            return "The specified value is not exist";
        case RC_NO_CHANGED_VALUE:
            return "No any value was changed";
        case RC_PERMISSION_DENIED:
            return "Permission denied";
        default:
            return "Unknown return code: " + std::to_string(rc);
    }
}

} // namespace gbhk

#endif // !GLOBAL_HOTKEY_RETURN_CODE_HPP
