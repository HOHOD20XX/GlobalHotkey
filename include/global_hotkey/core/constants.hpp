#ifndef GLOBAL_HOTKEY_CONSTANTS_HPP
#define GLOBAL_HOTKEY_CONSTANTS_HPP

namespace gbhk
{

// Return code.

// Success.
constexpr unsigned int _RC_SUCCESS = 0x00;
// Function is untimely be called.
// (e.g. too late or too early)
constexpr unsigned int _RC_UNTIMELY_CALL = 0x01;
// Function is be called by wrong thread.
// (e.g. #RegGlobalHotkey::add can't be called in work thread.)
constexpr unsigned int _RC_CALL_IN_WRONG_THREAD = 0x02;
// The specified value is not find.
// (e.g. a unregistered hotkey)
constexpr unsigned int _RC_NOT_FIND = 0x03;
// The specified value is already exists.
// (e.g. add a registered hotkey again)
constexpr unsigned int _RC_ALREADY_EXISTED = 0x04;
// The old value is equal to new value.
// (e.g. #RegGlobalHotkey::replace cant't use two identical #KeyCombination as oldkeycomb and newKeycomb.
constexpr unsigned int _RC_OLD_EQUAL_NEW = 0x05;

} // namespace gbhk

#endif // !GLOBAL_HOTKEY_CONSTANTS_HPP
