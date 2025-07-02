#ifndef GLOBAL_HOTKEY_RETURN_CODE_HPP
#define GLOBAL_HOTKEY_RETURN_CODE_HPP

#include <string>   // string, to_string

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
    RC_SUCCESS              = 0x00 | _RC_MASK,
    RC_BAD_TIMING           = 0x10 | _RC_MASK,
    RC_BAD_THREAD           = 0x11 | _RC_MASK,
};

inline std::string returnCodeMessage(int rc) noexcept
{
    /// @todo A clearer explanation is needed.
    switch (rc)
    {
        case RC_SUCCESS:
            return "Success";
        case RC_BAD_TIMING:
            return "Add, remove, replace and other change for global hotkey in Global Hotkey Manager, " \
                   "but GHM is not running yet";
        case RC_BAD_THREAD:
            return "Add, remove, replace and other change for global hotkey in Global Hotkey Manager, " \
                   "but the caller is in the same thread as GHM's worker thread";
        default:
            return "A return value depends on the platform or framwork " + std::to_string(rc);
    }
}

} // namespace gbhk

#endif // !GLOBAL_HOTKEY_RETURN_CODE_HPP
