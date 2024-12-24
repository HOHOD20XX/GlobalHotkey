#ifndef GLOBAL_HOTKEY_CONSTANTS_HPP
#define GLOBAL_HOTKEY_CONSTANTS_HPP

#include <string>   // to_string

#include "typealias.hpp"

namespace gbhk
{

enum ReturnCode : uchar
{
    RC_SUCCESS                  = 0x00,
    // Function is untimely be called.
    // (e.g. too late or too early)
    RC_UNTIMELY_CALL            = 0x01,
    // Function is be called by wrong thread.
    // (e.g. #RegGlobalHotkey::add() can't be called in work thread.)
    RC_CALL_IN_WRONG_THREAD     = 0x02,
    // The specified value is not find.
    // (e.g. a unregistered hotkey)
    RC_NOT_FIND                 = 0x03,
    // The specified value is already exists.
    // (e.g. add a registered hotkey again)
    RC_ALREADY_EXISTED          = 0x04,
    // The old value is equal to new value.
    // (e.g. #RegGlobalHotkey::replace() cant't use two identical #KeyCombination as oldkeycomb and newKeycomb.
    RC_OLD_EQUAL_NEW            = 0x05
};

inline String getReturnCodeMessage(uchar rc)
{
    switch (rc) {
        case RC_SUCCESS:
            return "Success.";
        case RC_UNTIMELY_CALL:
            return "The function is untimely be called (too late or too early), "
                   "e.g. call the #end() before call the #start().";
        case RC_CALL_IN_WRONG_THREAD:
            return "The function is be called by wrong thread, "
                   "e.g. #RegGlobalHotkey::add() can't be called in work thread.";
        case RC_NOT_FIND:
            return "The specified value is not be finded, "
                   "e.g. modify a unregistered hotkey.";
        case RC_ALREADY_EXISTED:
            return "The specified value is already exists, "
                   "e.g. Add a registered hotkey agian.";
        case RC_OLD_EQUAL_NEW:
            return "The specified value is equal to the value to be replacee, "
                   "e.g. Replace hotkey Ctrl+C to Ctrl+C.";
        default:
            return "The return code of external API (usually a system API): " + std::to_string(rc) + ".";
    }
}

} // namespace gbhk

#endif // !GLOBAL_HOTKEY_CONSTANTS_HPP
