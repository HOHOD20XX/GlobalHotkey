#ifndef MACROS_HPP
#define MACROS_HPP

// Return code

// Success.
#define GBHK_RSLT_SUCCESS                   0x00
// Function is untimely be called.
// (e.g. too late or too early)
#define GBHK_RSLT_UNTIMELY_CALL             0x01
// Function is be called by wrong thread.
// (e.g. #Regadd only can't be called in work thread.)
#define GBHK_RSLT_CALL_IN_WRONG_THREAD      0x02
// The specified value is not find.
// (e.g. a unregistered hotkey)
#define GBHK_RSLT_NOT_FIND                  0x03
// The specified value is already exists.
// (e.g. add a registered hotkey again)
#define GBHK_RSLT_ALREADY_EXISTED           0x04
// The old value is equal to new value.
// (e.g. #Regreplace cant't use two identical #KeyCombination as oldkeycomb and newKeycomb.
#define GBHK_RSLT_OLD_EQUAL_NEW             0x05

// String of modifier key
#define GBHK_MODKEY_TEXT_WIN        "Win"
#define GBHK_MODKEY_TEXT_CMD        "Command"
#define GBHK_MODKEY_TEXT_SUPER      "Super"
#define GBHK_MODKEY_TEXT_ALT        "Alt"
#define GBHK_MODKEY_TEXT_CTRL       "Ctrl"
#define GBHK_MODKEY_TEXT_SHIFT      "Shift"

#endif // !MACROS_HPP
