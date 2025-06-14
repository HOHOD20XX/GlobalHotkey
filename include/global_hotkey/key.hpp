#ifndef GLOBAL_HOTKEY_KEY_HPP
#define GLOBAL_HOTKEY_KEY_HPP

#include <initializer_list> // initializer_list
#include <string>           // string

#include "base.hpp"

namespace gbhk
{

enum ModifierFlag : int
{
    MODI_META           = 0X01,         // Win or Command or Super.
    MODI_CTRL           = 0X02,
    MODI_ALT            = 0X04,
    MODI_SHIFT          = 0X08,

    // Alias.
    MODI_WIN            = MODI_META,
    MODI_COMMAND        = MODI_META,
    MODI_SUPER          = MODI_META,
    MODI_OPTION         = MODI_ALT      // The Alt key on MacOS.
};

// To avoid conflicts with macro definitions, the KeyFlag naming style different from other enums in this library.
enum KeyFlag : int
{
    // Number keys.
    Key_0               = 0x0030,
    Key_1               = 0x0031,
    Key_2               = 0x0032,
    Key_3               = 0x0033,
    Key_4               = 0x0034,
    Key_5               = 0x0035,
    Key_6               = 0x0036,
    Key_7               = 0x0037,
    Key_8               = 0x0038,
    Key_9               = 0x0039,

    // Ascii alpha keys.
    Key_A               = 0x0041,
    Key_B               = 0x0042,
    Key_C               = 0x0043,
    Key_D               = 0x0044,
    Key_E               = 0x0045,
    Key_F               = 0x0046,
    Key_G               = 0x0047,
    Key_H               = 0x0048,
    Key_I               = 0x0049,
    Key_J               = 0x004a,
    Key_K               = 0x004b,
    Key_L               = 0x004c,
    Key_M               = 0x004d,
    Key_N               = 0x004e,
    Key_O               = 0x004f,
    Key_P               = 0x0050,
    Key_Q               = 0x0051,
    Key_R               = 0x0052,
    Key_S               = 0x0053,
    Key_T               = 0x0054,
    Key_U               = 0x0055,
    Key_V               = 0x0056,
    Key_W               = 0x0057,
    Key_X               = 0x0058,
    Key_Y               = 0x0059,
    Key_Z               = 0x005a,

    // Whitespace keys.
    Key_Tab             = 0x8000,
    Key_Space,
    Key_Enter,

    // Navigation keys.
    Key_Home,
    Key_End,
    Key_Page_Up,
    Key_Page_Down,
    Key_Left,
    Key_Up,
    Key_Right,
    Key_Down,

    // Editing keys.
    Key_Backspace,
    Key_Insert,
    Key_Delete,
    Key_Clear,

    // Function keys.
    Key_F1,
    Key_F2,
    Key_F3,
    Key_F4,
    Key_F5,
    Key_F6,
    Key_F7,
    Key_F8,
    Key_F9,
    Key_F10,
    Key_F11,
    Key_F12,
    Key_F13,
    Key_F14,
    Key_F15,
    Key_F16,
    Key_F17,
    Key_F18,
    Key_F19,
    Key_F20,
    Key_F21,
    Key_F22,
    Key_F23,
    Key_F24,

    // Numpad number keys.
    Key_Numpad_0,
    Key_Numpad_1,
    Key_Numpad_2,
    Key_Numpad_3,
    Key_Numpad_4,
    Key_Numpad_5,
    Key_Numpad_6,
    Key_Numpad_7,
    Key_Numpad_8,
    Key_Numpad_9,

    // Numpad operator keys.
    Key_Numpad_Add,         // '+' on numpad.
    Key_Numpad_Subtract,    // '-' on numpad.
    Key_Numpad_Multiply,    // '*' or '×' on numpad.
    Key_Numpad_Divide,      // '/' or '÷' on numpad.
    Key_Numpad_Decimal,     // '.' on numpad.
    Key_Numpad_Separator,   // ',' on numpad.

    // Applications keys.
    Key_Escape,
    Key_Pause,              // Not is MEDIA_PLAY_PAUSE.
    Key_Play,               // Not is MEDIA_PLAY_PAUSE. Maybe be removed in future.
    Key_Help,

    // Media keys.
    Key_Media_Next,
    Key_Media_Previous,
    Key_Media_Play_Pause,
    Key_Media_Stop,

    // Volume keys.
    Key_Volume_Up,
    Key_Volume_Down,
    Key_Volume_Mute,

    // Lock keys.
    Key_Caps_Lock,
    Key_Num_Lock,
    Key_Scroll_Lock,

    // Misc keys.
    Key_Print_Screen,
    Key_Menu,

    // OEM keys.
    Key_Left_Quote,         // '`~' on US standard keyboard.
    Key_Minus,              // '-_' on US standard keyboard.
    Key_Equal,              // '=+' on US standard keyboard.
    Key_Left_Bracket,       // '[{' on US standard keyboard.
    Key_Right_Bracket,      // ']}' on US standard keyboard.
    Key_Semicolon,          // ';:' on US standard keyboard.
    Key_Apostrophe,         // ''"' on US standard keyboard.
    Key_Comma,              // ',<' on US standard keyboard.
    Key_Period,             // '.>' on US standard keyboard.
    Key_Slash,              // '/?' on US standard keyboard.
    Key_Backslash,          // '\|' on US standard keyboard.
    Key_Angle_Bracket,      // '<>' or '\|' on RT 102-key keyboard.

    // Modifier keys.
    // Use the key combination with these keys as a hotkey is useless.)
    // These keys only be used to keyboard hook event. (e.g. the #addKeyListener() in the key_board module.)
    Key_Modi_Meta,
    Key_Modi_Meta_Left,
    Key_Modi_Meta_Right,
    Key_Modi_Ctrl,
    Key_Modi_Ctrl_Left,
    Key_Modi_Ctrl_Right,
    Key_Modi_Alt,
    Key_Modi_Alt_Left,
    Key_Modi_Alt_Right,
    Key_Modi_Shift,
    Key_Modi_Shift_Left,
    Key_Modi_Shift_Right,

    // Alias.
    Key_Modi_Option         = Key_Modi_Alt,
    Key_Modi_Option_Left    = Key_Modi_Alt_Left,
    Key_Modi_Option_Right   = Key_Modi_Alt_Right
};

class Modifiers
{
public:
    constexpr inline Modifiers() noexcept {}
    constexpr inline Modifiers(ModifierFlag modifier) noexcept : i_(modifier) {}
    constexpr inline Modifiers(int modifiers) noexcept : i_(modifiers) {}
    constexpr inline Modifiers(const std::initializer_list<ModifierFlag>& modifiers) noexcept :
        i_(initializerListHelper_(modifiers.begin(), modifiers.end())) {}

    constexpr inline operator int() const noexcept { return i_; }
    constexpr inline int value() const noexcept { return i_; }

    constexpr inline bool has(ModifierFlag modifier) const noexcept { return (i_ & modifier) != 0; }
    constexpr inline bool has(Modifiers modifiers) const noexcept { return (i_ & modifiers.i_) == modifiers.i_; }
    /// @brief Check if the modifiers has any modifiers.
    constexpr inline bool isValid() const noexcept
    {
        return (i_ & MODI_META) || (i_ & MODI_CTRL) || (i_ & MODI_ALT) || (i_ & MODI_SHIFT);
    }

#if _GLOBAL_HOTKEY_CPPVERS >= 201703L
    // In C++17, constexpr member functions are no longer implicitly const.
    constexpr inline Modifiers& add(ModifierFlag modifier) noexcept { i_ |= modifier; return *this; }
    constexpr inline Modifiers& add(Modifiers modifiers) noexcept { i_ |= modifiers.i_; return *this; }
    constexpr inline Modifiers& remove(ModifierFlag modifier) noexcept { i_ &= ~modifier; return *this; }
    constexpr inline Modifiers& remove(Modifiers modifiers) noexcept { i_ &= ~modifiers.i_; return *this; }
#else
    inline Modifiers& add(ModifierFlag modifier) noexcept { i_ |= modifier; return *this; }
    inline Modifiers& add(Modifiers modifiers) noexcept { i_ |= modifiers.i_; return *this; }
    inline Modifiers& remove(ModifierFlag modifier) noexcept { i_ &= ~modifier; return *this; }
    inline Modifiers& remove(Modifiers modifiers) noexcept { i_ &= ~modifiers.i_; return *this; }

#endif // _GLOBAL_HOTKEY_CPPVERS >= 201703L

    friend constexpr inline bool operator==(Modifiers lhs, Modifiers rhs) noexcept { return lhs.i_ == rhs.i_; }
    friend constexpr inline bool operator!=(Modifiers lhs, Modifiers rhs) noexcept { return lhs.i_ != rhs.i_; }
    friend constexpr inline bool operator==(Modifiers lhs, ModifierFlag rhs) noexcept { return lhs.i_ == rhs; }
    friend constexpr inline bool operator!=(Modifiers lhs, ModifierFlag rhs) noexcept { return lhs.i_ != rhs; }
    friend constexpr inline bool operator==(ModifierFlag lhs, Modifiers rhs) noexcept { return lhs == rhs.i_; }
    friend constexpr inline bool operator!=(ModifierFlag lhs, Modifiers rhs) noexcept { return lhs != rhs.i_; }
    friend constexpr inline bool operator==(Modifiers lhs, int rhs) noexcept { return lhs.i_ == rhs; }
    friend constexpr inline bool operator!=(Modifiers lhs, int rhs) noexcept { return lhs.i_ != rhs; }
    friend constexpr inline bool operator==(int lhs, Modifiers rhs) noexcept { return lhs == rhs.i_; }
    friend constexpr inline bool operator!=(int lhs, Modifiers rhs) noexcept { return lhs != rhs.i_; }

private:
    constexpr static inline int initializerListHelper_(std::initializer_list<ModifierFlag>::const_iterator it,
                                                       std::initializer_list<ModifierFlag>::const_iterator end)
    noexcept
    {
        return (it == end ? 0 : (*it | initializerListHelper_(it + 1, end)));
    }

    int i_ = 0;
};

class Key
{
public:
    constexpr inline Key() noexcept {}
    constexpr inline Key(KeyFlag key) noexcept : i_(key) {}
    constexpr inline Key(char key) noexcept : i_(toUpper_(key)) {}
    constexpr inline Key(int key) noexcept : i_(key) {}

    constexpr inline operator int() const noexcept { return i_; }
    constexpr inline int value() const noexcept { return i_; }

    /// @brief Check if the key value is 0.
    constexpr inline bool isValid() const noexcept { return i_ != 0; }

    friend constexpr inline bool operator==(Key lhs, Key rhs) noexcept { return lhs.i_ == rhs.i_; }
    friend constexpr inline bool operator!=(Key lhs, Key rhs) noexcept { return lhs.i_ != rhs.i_; }
    friend constexpr inline bool operator==(Key lhs, KeyFlag rhs) noexcept { return lhs.i_ == rhs; }
    friend constexpr inline bool operator!=(Key lhs, KeyFlag rhs) noexcept { return lhs.i_ != rhs; }
    friend constexpr inline bool operator==(KeyFlag lhs, Key rhs) noexcept { return lhs == rhs.i_; }
    friend constexpr inline bool operator!=(KeyFlag lhs, Key rhs) noexcept { return lhs != rhs.i_; }
    friend constexpr inline bool operator==(Key lhs, char rhs) noexcept { return lhs.i_ == Key::toUpper_(rhs); }
    friend constexpr inline bool operator!=(Key lhs, char rhs) noexcept { return lhs.i_ != Key::toUpper_(rhs); }
    friend constexpr inline bool operator==(char lhs, Key rhs) noexcept { return Key::toUpper_(lhs) == rhs.i_; }
    friend constexpr inline bool operator!=(char lhs, Key rhs) noexcept { return Key::toUpper_(lhs) != rhs.i_; }
    friend constexpr inline bool operator==(Key lhs, int rhs) noexcept { return lhs.i_ == rhs; }
    friend constexpr inline bool operator!=(Key lhs, int rhs) noexcept { return lhs.i_ != rhs; }
    friend constexpr inline bool operator==(int lhs, Key rhs) noexcept { return lhs == rhs.i_; }
    friend constexpr inline bool operator!=(int lhs, Key rhs) noexcept { return lhs != rhs.i_; }

private:
    constexpr static inline int toUpper_(char key) noexcept
    {
        return ((key >= 'a' && key <= 'z') ? key - 'a' + 'A' : key);
    }

    int i_ = 0;
};

GBHK_API std::string modifiersString(const Modifiers& modifiers, bool isPrettySpace = false) noexcept;

GBHK_API std::string keyString(const Key& key) noexcept;

GBHK_API Modifiers getModifiersFromString(const std::string& str) noexcept;

GBHK_API Key getKeyFromString(const std::string& str) noexcept;

GBHK_API Modifiers getModifiersFromNative(int nativeModifiers) noexcept;

GBHK_API Key getKeyFromNative(int nativeKey) noexcept;

GBHK_API int getNativeModifiers(const Modifiers& modifiers) noexcept;

GBHK_API int getNativeKey(const Key& key) noexcept;

}

#endif // !GLOBAL_HOTKEY_KEY_HPP
