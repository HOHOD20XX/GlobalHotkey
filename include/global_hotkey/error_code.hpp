#ifndef GLOBAL_HOTKEY_ERROR_CODE_HPP
#define GLOBAL_HOTKEY_ERROR_CODE_HPP

#include <string>   // to_string

#include "base.hpp"

namespace gbhk
{

#ifdef _GLOBAL_HOTKEY_WIN
    constexpr int _EC_MASK = 1 << 29;
#elif defined(_GLOBAL_HOTKEY_MAC)
    constexpr int _EC_MASK = 0;
#elif defined(_GLOBAL_HOTKEY_LINUX)
    constexpr int _EC_MASK = 0;
#else
    constexpr int _EC_MASK = 0;
#endif // _GLOBAL_HOTKEY_WIN

enum ErrorCode : int
{
    EC_SUCCESS                      = 0x00 | _EC_MASK,

    EC_ARR_GHM_IN_WRONG_TIME        = 0x10 | _EC_MASK,
    EC_ARR_GHM_IN_WRONG_THREAD      = 0x11 | _EC_MASK,
    EC_END_GHM_IN_WRONG_THREAD      = 0x12 | _EC_MASK,

    EC_INVALID_VALUE                = 0x20 | _EC_MASK,
    EC_EXIST_SAME_VALUE             = 0x21 | _EC_MASK,
    EC_NO_SPECIFIED_VALUE           = 0x22 | _EC_MASK,
    EC_NO_CHANGED_VALUE             = 0x23 | _EC_MASK
};

inline String errorCodeMessage(int ec) noexcept
{
    switch (ec)
    {
        case EC_SUCCESS:
            return "Success";
        case EC_ARR_GHM_IN_WRONG_TIME:
            return "Add, remove or replace global hotkey for the Global Hotkey Manager, " \
                   "but GHM is not running yet";
        case EC_ARR_GHM_IN_WRONG_THREAD:
            return "Add, remove or replace global hotkey for the Global Hotkey Manager, " \
                   "but the caller is in the same thread as GHM's work thread";
        case EC_END_GHM_IN_WRONG_THREAD:
            return "End the Global Hotkey Manager in the same thread as GHM's work thread";
        case EC_INVALID_VALUE:
            return "Invalid value";
        case EC_EXIST_SAME_VALUE:
            return "Already exist same value";
        case EC_NO_SPECIFIED_VALUE:
            return "The specified value is not exist";
        case EC_NO_CHANGED_VALUE:
            return "No any value was changed";
        default:
            return "Unknown error code: " + std::to_string(ec);
    }
}

} // namespace gbhk

#endif // !GLOBAL_HOTKEY_ERROR_CODE_HPP
