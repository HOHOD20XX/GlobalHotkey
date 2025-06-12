#ifndef GLOBAL_HOTKEY_KEY_HPP
#define GLOBAL_HOTKEY_KEY_HPP

#include "base.hpp"

namespace gbhk
{

enum ModifierFlag : int
{
    MODI_META           = 0X01,        // Win or Command or Super.
    MODI_CTRL           = 0X02,
    MODI_ALT            = 0X04,
    MODI_SHIFT          = 0X08,

    // Alias.
    MODI_WIN            = MODI_META,
    MODI_COMMAND        = MODI_META,
    MODI_SUPER          = MODI_META,
    MODI_OPTION         = MODI_ALT     // The Alt key on MacOS.
};

enum KeyFlag : int
{
    // Number keys.
    KEY_0               = 0x0030,
    KEY_1               = 0x0031,
    KEY_2               = 0x0032,
    KEY_3               = 0x0033,
    KEY_4               = 0x0034,
    KEY_5               = 0x0035,
    KEY_6               = 0x0036,
    KEY_7               = 0x0037,
    KEY_8               = 0x0038,
    KEY_9               = 0x0039,

    // Ascii alpha keys.
    KEY_A               = 0x0041,
    KEY_B               = 0x0042,
    KEY_C               = 0x0043,
    KEY_D               = 0x0044,
    KEY_E               = 0x0045,
    KEY_F               = 0x0046,
    KEY_G               = 0x0047,
    KEY_H               = 0x0048,
    KEY_I               = 0x0049,
    KEY_J               = 0x004a,
    KEY_K               = 0x004b,
    KEY_L               = 0x004c,
    KEY_M               = 0x004d,
    KEY_N               = 0x004e,
    KEY_O               = 0x004f,
    KEY_P               = 0x0050,
    KEY_Q               = 0x0051,
    KEY_R               = 0x0052,
    KEY_S               = 0x0053,
    KEY_T               = 0x0054,
    KEY_U               = 0x0055,
    KEY_V               = 0x0056,
    KEY_W               = 0x0057,
    KEY_X               = 0x0058,
    KEY_Y               = 0x0059,
    KEY_Z               = 0x005a,

    // Whitespace keys.
    KEY_TAB             = 0x8000,
    KEY_SPACE,
    KEY_ENTER,

    // Navigation keys.
    KEY_HOME,
    KEY_END,
    KEY_PAGE_UP,
    KEY_PAGE_DOWN,
    KEY_LEFT,
    KEY_UP,
    KEY_RIGHT,
    KEY_DOWN,

    // Editing keys.
    KEY_BACKSPACE,
    KEY_INSERT,
    KEY_DELETE,
    KEY_CLEAR,

    // Function keys.
    KEY_F1,
    KEY_F2,
    KEY_F3,
    KEY_F4,
    KEY_F5,
    KEY_F6,
    KEY_F7,
    KEY_F8,
    KEY_F9,
    KEY_F10,
    KEY_F11,
    KEY_F12,
    KEY_F13,
    KEY_F14,
    KEY_F15,
    KEY_F16,
    KEY_F17,
    KEY_F18,
    KEY_F19,
    KEY_F20,
    KEY_F21,
    KEY_F22,
    KEY_F23,
    KEY_F24,

    // Numpad number keys.
    KEY_NUMPAD_0,
    KEY_NUMPAD_1,
    KEY_NUMPAD_2,
    KEY_NUMPAD_3,
    KEY_NUMPAD_4,
    KEY_NUMPAD_5,
    KEY_NUMPAD_6,
    KEY_NUMPAD_7,
    KEY_NUMPAD_8,
    KEY_NUMPAD_9,

    // Numpad operator keys.
    KEY_NUMPAD_ADD,         // '+' on numpad.
    KEY_NUMPAD_SUBTRACT,    // '-' on numpad.
    KEY_NUMPAD_MULTIPLY,    // '*' or '×' on numpad.
    KEY_NUMPAD_DIVIDE,      // '/' or '÷' on numpad.
    KEY_NUMPAD_DECIMAL,     // '.' on numpad.
    KEY_NUMPAD_SEPARATOR,   // ',' on numpad.

    // Applications keys.
    KEY_ESCAPE,
    KEY_PAUSE,              // Not is MEDIA_PLAY_PAUSE.
    KEY_PLAY,               // Not is MEDIA_PLAY_PAUSE. Maybe be removed in future.
    KEY_HELP,

    // Media keys.
    KEY_MEDIA_NEXT,
    KEY_MEDIA_PREVIOUS,
    KEY_MEDIA_PLAY_PAUSE,
    KEY_MEDIA_STOP,

    // Volume keys.
    KEY_VOLUME_UP,
    KEY_VOLUME_DOWN,
    KEY_VOLUME_MUTE,

    // Lock keys.
    KEY_CAPS_LOCK,
    KEY_NUM_LOCK,
    KEY_SCROLL_LOCK,

    // Misc keys.
    KEY_PRINT_SCREEN,
    KEY_MENU,

    // OEM keys.
    KEY_LEFT_QUOTE,         // '`~' on US standard keyboard.
    KEY_MINUS,              // '-_' on US standard keyboard.
    KEY_EQUAL,              // '=+' on US standard keyboard.
    KEY_LEFT_BRACKET,       // '[{' on US standard keyboard.
    KEY_RIGHT_BRACKET,      // ']}' on US standard keyboard.
    KEY_SEMICOLON,          // ';:' on US standard keyboard.
    KEY_APOSTROPHE,         // ''"' on US standard keyboard.
    KEY_COMMA,              // ',<' on US standard keyboard.
    KEY_PERIOD,             // '.>' on US standard keyboard.
    KEY_SLASH,              // '/?' on US standard keyboard.
    KEY_BACKSLASH,          // '\|' on US standard keyboard.
    KEY_ANGLE_BRACKET,      // '<>' or '\|' on RT 102-key keyboard.

    // Modifier keys.
    // Use the key combination with these keys as a hotkey is useless.)
    // These keys only be used to keyboard hook event. (e.g. the #addKeyListener() in the key_board module.)
    KEY_MODI_META,
    KEY_MODI_META_LEFT,
    KEY_MODI_META_RIGHT,
    KEY_MODI_CTRL,
    KEY_MODI_CTRL_LEFT,
    KEY_MODI_CTRL_RIGHT,
    KEY_MODI_ALT,
    KEY_MODI_ALT_LEFT,
    KEY_MODI_ALT_RIGHT,
    KEY_MODI_SHIFT,
    KEY_MODI_SHIFT_LEFT,
    KEY_MODI_SHIFT_RIGHT,

    // Alias.
    KEY_MODI_OPTION         = KEY_MODI_ALT,
    KEY_MODI_OPTION_LEFT    = KEY_MODI_ALT_LEFT,
    KEY_MODI_OPTION_RIGHT   = KEY_MODI_ALT_RIGHT
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

GBHK_API String modifiersString(const Modifiers& modifiers, bool isPrettySpace = false) noexcept;

GBHK_API String keyString(const Key& key) noexcept;

GBHK_API Modifiers getModifiersFromString(const String& str) noexcept;

GBHK_API Key getKeyFromString(const String& str) noexcept;

GBHK_API Modifiers getModifiersFromNative(int nativeModifiers) noexcept;

GBHK_API Key getKeyFromNative(int nativeKey) noexcept;

GBHK_API int getNativeModifiers(const Modifiers& modifiers) noexcept;

GBHK_API int getNativeKey(const Key& key) noexcept;

}

#endif // !GLOBAL_HOTKEY_KEY_HPP
