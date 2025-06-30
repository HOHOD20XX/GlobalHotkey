#include <cctype>   // isspace, isalnum, toupper
#include <sstream>  // stringstream

#include <global_hotkey/key.hpp>

#define IS_SPACE(c) std::isspace(static_cast<unsigned char>(c))
#define IS_ALNUM(c) std::isalnum(static_cast<unsigned char>(c))
#define TO_UPPER(c) std::toupper(static_cast<unsigned char>(c))

#ifdef _GLOBAL_HOTKEY_WIN
    #define META_TEXT   "Win"
    #define ALT_TEXT    "Alt"
#elif defined(_GLOBAL_HOTKEY_MAC)
    #define META_TEXT   "Command"
    #define ALT_TEXT    "Option"
#elif defined(_GLOBAL_HOTKEY_LINUX)
    #define META_TEXT   "Super"
    #define ALT_TEXT    "Alt"
#else
    #define META_TEXT   "Meta"
    #define ALT_TEXT    "Alt"
#endif // _GLOBAL_HOTKEY_WIN

#define CTRL_TEXT   "Ctrl"
#define SHIFT_TEXT  "Shift"

#define LEFT_TEXT   "Left"
#define RIGHT_TEXT  "Right"

namespace gbhk
{

static std::string modifierFlagString(ModifierFlag flag) noexcept
{
    switch (flag)
    {
        case META:  return META_TEXT;
        case CTRL:  return CTRL_TEXT;
        case ALT:   return ALT_TEXT;
        case SHIFT: return SHIFT_TEXT;
        default:    return "";
    }
}

GBHK_API std::string modifiersString(const Modifiers& modifiers, bool isPrettySpace) noexcept
{
    std::string rslt;
    std::string separator = isPrettySpace ? " + " : "+";

    if (modifiers.has(META))
        rslt += modifierFlagString(META);
    if (modifiers.has(CTRL))
        rslt += (!rslt.empty() ? separator : "") + modifierFlagString(CTRL);
    if (modifiers.has(ALT))
        rslt += (!rslt.empty() ? separator : "") + modifierFlagString(ALT);
    if (modifiers.has(SHIFT))
        rslt += (!rslt.empty() ? separator : "") + modifierFlagString(SHIFT);

    return rslt;
}

GBHK_API std::string keyString(const Key& key) noexcept
{
    switch (key)
    {
        // Number keys.
        case Key_0:                 return "0";
        case Key_1:                 return "1";
        case Key_2:                 return "2";
        case Key_3:                 return "3";
        case Key_4:                 return "4";
        case Key_5:                 return "5";
        case Key_6:                 return "6";
        case Key_7:                 return "7";
        case Key_8:                 return "8";
        case Key_9:                 return "9";

        // ASCII letter keys.
        case Key_A:                 return "A";
        case Key_B:                 return "B";
        case Key_C:                 return "C";
        case Key_D:                 return "D";
        case Key_E:                 return "E";
        case Key_F:                 return "F";
        case Key_G:                 return "G";
        case Key_H:                 return "H";
        case Key_I:                 return "I";
        case Key_J:                 return "J";
        case Key_K:                 return "K";
        case Key_L:                 return "L";
        case Key_M:                 return "M";
        case Key_N:                 return "N";
        case Key_O:                 return "O";
        case Key_P:                 return "P";
        case Key_Q:                 return "Q";
        case Key_R:                 return "R";
        case Key_S:                 return "S";
        case Key_T:                 return "T";
        case Key_U:                 return "U";
        case Key_V:                 return "V";
        case Key_W:                 return "W";
        case Key_X:                 return "X";
        case Key_Y:                 return "Y";
        case Key_Z:                 return "Z";

        // Whitespace keys.
        case Key_Tab:               return "Tab";
        case Key_Space:             return "Space";
        case Key_Enter:             return "Enter";

        // Navigation keys.
        case Key_Home:              return "Home";
        case Key_End:               return "End";
        case Key_Page_Up:           return "Page Up";
        case Key_Page_Down:         return "Page Down";
        case Key_Left:              return "Left";
        case Key_Up:                return "Up";
        case Key_Right:             return "Right";
        case Key_Down:              return "Down";

        // Editing keys.
        case Key_Backspace:         return "Backspace";
        case Key_Insert:            return "Insert";
        case Key_Delete:            return "Delete";
        case Key_Clear:             return "Clear";

        // Function keys.
        case Key_F1:                return "F1";
        case Key_F2:                return "F2";
        case Key_F3:                return "F3";
        case Key_F4:                return "F4";
        case Key_F5:                return "F5";
        case Key_F6:                return "F6";
        case Key_F7:                return "F7";
        case Key_F8:                return "F8";
        case Key_F9:                return "F9";
        case Key_F10:               return "F10";
        case Key_F11:               return "F11";
        case Key_F12:               return "F12";
        case Key_F13:               return "F13";
        case Key_F14:               return "F14";
        case Key_F15:               return "F15";
        case Key_F16:               return "F16";
        case Key_F17:               return "F17";
        case Key_F18:               return "F18";
        case Key_F19:               return "F19";
        case Key_F20:               return "F20";
        case Key_F21:               return "F21";
        case Key_F22:               return "F22";
        case Key_F23:               return "F23";
        case Key_F24:               return "F24";

        // Numpad number keys.
        case Key_Numpad_0:          return "Numpad 0";
        case Key_Numpad_1:          return "Numpad 1";
        case Key_Numpad_2:          return "Numpad 2";
        case Key_Numpad_3:          return "Numpad 3";
        case Key_Numpad_4:          return "Numpad 4";
        case Key_Numpad_5:          return "Numpad 5";
        case Key_Numpad_6:          return "Numpad 6";
        case Key_Numpad_7:          return "Numpad 7";
        case Key_Numpad_8:          return "Numpad 8";
        case Key_Numpad_9:          return "Numpad 9";

        // Numpad operator keys.
        case Key_Numpad_Add:        return "Numpad Add";
        case Key_Numpad_Subtract:   return "Numpad Subtract";
        case Key_Numpad_Multiply:   return "Numpad Multiply";
        case Key_Numpad_Divide:     return "Numpad Divide";
        case Key_Numpad_Decimal:    return "Numpad Decimal";
        case Key_Numpad_Separator:  return "Numpad Separator";

        // Applications keys.
        case Key_Escape:            return "Esc";
        case Key_Pause:             return "Pause";
        case Key_Help:              return "Help";

        // Media keys.
        case Key_Media_Next:        return "Media Next";
        case Key_Media_Previous:    return "Media Previous";
        case Key_Media_Play_Pause:  return "Media Play/Pause";
        case Key_Media_Stop:        return "Media Stop";

        // Volume keys.
        case Key_Volume_Up:         return "Volume Up";
        case Key_Volume_Down:       return "Volume Down";
        case Key_Volume_Mute:       return "Volume Mute";

        // Lock keys.
        case Key_Caps_Lock:         return "CapsLock";
        case Key_Num_Lock:          return "NumLock";
        case Key_Scroll_Lock:       return "ScrollLock";

        // Misc keys.
        case Key_Print_Screen:      return "PrintScreen";
        case Key_Menu:              return "Menu";

        // OEM keys.
        case Key_Left_Quote:        return "`";
        case Key_Minus:             return "-";
        case Key_Equal:             return "=";
        case Key_Left_Bracket:      return "[";
        case Key_Right_Bracket:     return "]";
        case Key_Semicolon:         return ";";
        case Key_Apostrophe:        return "'";
        case Key_Comma:             return ",";
        case Key_Period:            return ".";
        case Key_Slash:             return "/";
        case Key_Backslash:         return "\\";
        case Key_Angle_Bracket:     return "<>";

        // Modifier keys.
        case Key_Mod_Meta:          return META_TEXT;
        case Key_Mod_Meta_Left:     return (LEFT_TEXT " " META_TEXT);
        case Key_Mod_Meta_Right:    return (RIGHT_TEXT " " META_TEXT);
        case Key_Mod_Ctrl:          return CTRL_TEXT;
        case Key_Mod_Ctrl_Left:     return (LEFT_TEXT " " CTRL_TEXT);
        case Key_Mod_Ctrl_Right:    return (RIGHT_TEXT " " CTRL_TEXT);
        case Key_Mod_Alt:           return ALT_TEXT;
        case Key_Mod_Alt_Left:      return (LEFT_TEXT " " ALT_TEXT);
        case Key_Mod_Alt_Right:     return (RIGHT_TEXT " " ALT_TEXT);
        case Key_Mod_Shift:         return SHIFT_TEXT;
        case Key_Mod_Shift_Left:    return (LEFT_TEXT " " SHIFT_TEXT);
        case Key_Mod_Shift_Right:   return (RIGHT_TEXT " " SHIFT_TEXT);

        default:                    return "";
    }
}

// Check whether two strings is equal, ignoring case, spaces and underscores.
static bool isEqualStr(const std::string& str1, const std::string& str2) noexcept
{
    size_t i = 0, j = 0;
    while (i < str1.size() || j < str2.size())
    {
        while (i < str1.size() && (IS_SPACE(str1[i]) || str1[i] == '_')) i++;
        while (j < str2.size() && (IS_SPACE(str2[j]) || str2[j] == '_')) j++;

        if (i >= str1.size() || j >= str2.size())
            return (i >= str1.size() && j >= str2.size());

        char c1 = TO_UPPER(str1[i]);
        char c2 = TO_UPPER(str2[j]);
        if (c1 != c2)
            return false;

        i++;
        j++;
    }

    return true;
}

static int getModifierFlagFromString(const std::string& str) noexcept
{
    if (isEqualStr(str, "win") || isEqualStr(str, "command") ||
        isEqualStr(str, "super") || isEqualStr(str, "meta"))
        return META;
    if (isEqualStr(str, "ctrl") || isEqualStr(str, "control"))
        return CTRL;
    if (isEqualStr(str, "alt") || isEqualStr(str, "option"))
        return ALT;
    if (isEqualStr(str, "shift"))
        return SHIFT;
    return 0;
}

GBHK_API Modifiers getModifiersFromString(const std::string& str) noexcept
{
    std::stringstream ss;
    ss << str;
    Modifiers rslt;
    std::string s;
    while (std::getline(ss, s, '+'))
        rslt.add(getModifierFlagFromString(s));
    return rslt;
}

GBHK_API Key getKeyFromString(const std::string& str) noexcept
{
    if (str.empty())
        return Key();

    if (str.size() == 1 && IS_ALNUM(str[0]))
        return Key(str[0]);

    // Whitespace keys.
    if (isEqualStr(str, "tab"))                 return Key_Tab;
    if (isEqualStr(str, "space"))               return Key_Space;
    if (isEqualStr(str, "enter") || isEqualStr(str, "return"))
        return Key_Enter;

    // Navigation keys.
    if (isEqualStr(str, "home"))                return Key_Home;
    if (isEqualStr(str, "end"))                 return Key_End;
    if (isEqualStr(str, "page up") || isEqualStr(str, "pgup"))
        return Key_Page_Up;
    if (isEqualStr(str, "page down") || isEqualStr(str, "pgdn"))
        return Key_Page_Down;
    if (isEqualStr(str, "left") || isEqualStr(str, "arrow left"))
        return Key_Left;
    if (isEqualStr(str, "up") || isEqualStr(str, "arrow up"))
        return Key_Up;
    if (isEqualStr(str, "right") || isEqualStr(str, "arrow right"))
        return Key_Right;
    if (isEqualStr(str, "down") || isEqualStr(str, "arrow down"))
        return Key_Down;

    // Editing keys.
    if (isEqualStr(str, "backspace") || isEqualStr(str, "back"))
        return Key_Backspace;
    if (isEqualStr(str, "insert") || isEqualStr(str, "ins"))
        return Key_Insert;
    if (isEqualStr(str, "delete") || isEqualStr(str, "del"))
        return Key_Delete;
    if (isEqualStr(str, "clear"))               return Key_Clear;

    // Function keys.
    if (isEqualStr(str, "f1"))                  return Key_F1;
    if (isEqualStr(str, "f2"))                  return Key_F2;
    if (isEqualStr(str, "f3"))                  return Key_F3;
    if (isEqualStr(str, "f4"))                  return Key_F4;
    if (isEqualStr(str, "f5"))                  return Key_F5;
    if (isEqualStr(str, "f6"))                  return Key_F6;
    if (isEqualStr(str, "f7"))                  return Key_F7;
    if (isEqualStr(str, "f8"))                  return Key_F8;
    if (isEqualStr(str, "f9"))                  return Key_F9;
    if (isEqualStr(str, "f10"))                 return Key_F10;
    if (isEqualStr(str, "f11"))                 return Key_F11;
    if (isEqualStr(str, "f12"))                 return Key_F12;
    if (isEqualStr(str, "f13"))                 return Key_F13;
    if (isEqualStr(str, "f14"))                 return Key_F14;
    if (isEqualStr(str, "f15"))                 return Key_F15;
    if (isEqualStr(str, "f16"))                 return Key_F16;
    if (isEqualStr(str, "f17"))                 return Key_F17;
    if (isEqualStr(str, "f18"))                 return Key_F18;
    if (isEqualStr(str, "f19"))                 return Key_F19;
    if (isEqualStr(str, "f20"))                 return Key_F20;
    if (isEqualStr(str, "f21"))                 return Key_F21;
    if (isEqualStr(str, "f22"))                 return Key_F22;
    if (isEqualStr(str, "f23"))                 return Key_F23;
    if (isEqualStr(str, "f24"))                 return Key_F24;

    // Numpad number keys.
    if (isEqualStr(str, "numpad 0") || isEqualStr(str, "keypad 0"))
        return Key_Numpad_0;
    if (isEqualStr(str, "numpad 1") || isEqualStr(str, "keypad 1"))
        return Key_Numpad_1;
    if (isEqualStr(str, "numpad 2") || isEqualStr(str, "keypad 2"))
        return Key_Numpad_2;
    if (isEqualStr(str, "numpad 3") || isEqualStr(str, "keypad 3"))
        return Key_Numpad_3;
    if (isEqualStr(str, "numpad 4") || isEqualStr(str, "keypad 4"))
        return Key_Numpad_4;
    if (isEqualStr(str, "numpad 5") || isEqualStr(str, "keypad 5"))
        return Key_Numpad_5;
    if (isEqualStr(str, "numpad 6") || isEqualStr(str, "keypad 6"))
        return Key_Numpad_6;
    if (isEqualStr(str, "numpad 7") || isEqualStr(str, "keypad 7"))
        return Key_Numpad_7;
    if (isEqualStr(str, "numpad 8") || isEqualStr(str, "keypad 8"))
        return Key_Numpad_8;
    if (isEqualStr(str, "numpad 9") || isEqualStr(str, "keypad 9"))
        return Key_Numpad_9;

    // Numpad operator keys.
    if (isEqualStr(str, "numpad add") || isEqualStr(str, "keypad add"))
        return Key_Numpad_Add;
    if (isEqualStr(str, "numpad subtract") || isEqualStr(str, "keypad subtract"))
        return Key_Numpad_Subtract;
    if (isEqualStr(str, "numpad multiply") || isEqualStr(str, "keypad multiply"))
        return Key_Numpad_Multiply;
    if (isEqualStr(str, "numpad divide") || isEqualStr(str, "keypad divide"))
        return Key_Numpad_Divide;
    if (isEqualStr(str, "numpad decimal") || isEqualStr(str, "keypad decimal"))
        return Key_Numpad_Decimal;
    if (isEqualStr(str, "numpad separator") || isEqualStr(str, "keypad separator"))
        return Key_Numpad_Separator;

    // Applications keys.
    if (isEqualStr(str, "esc") || isEqualStr(str, "escape"))
        return Key_Escape;
    if (isEqualStr(str, "pause"))               return Key_Pause;
    if (isEqualStr(str, "help"))                return Key_Help;

    // Media keys.
    if (isEqualStr(str, "media next") || isEqualStr(str, "media track next"))
        return Key_Media_Next;
    if (isEqualStr(str, "media previous") || isEqualStr(str, "media prev") ||
        isEqualStr(str, "media track previous") || isEqualStr(str, "media track prev"))
        return Key_Media_Previous;
    if (isEqualStr(str, "media play/pause") || isEqualStr(str, "media play pause"))
        return Key_Media_Play_Pause;
    if (isEqualStr(str, "media stop"))          return Key_Media_Stop;

    // Volume keys.
    if (isEqualStr(str, "volume up") || isEqualStr(str, "vol up"))
        return Key_Volume_Up;
    if (isEqualStr(str, "volume down") || isEqualStr(str, "vol down"))
        return Key_Volume_Down;
    if (isEqualStr(str, "volume mute") || isEqualStr(str, "vol mute"))
        return Key_Volume_Mute;

    // Lock keys.
    if (isEqualStr(str, "caps lock") || isEqualStr(str, "caps lk"))
        return Key_Caps_Lock;
    if (isEqualStr(str, "num lock") || isEqualStr(str, "num lk"))
        return Key_Num_Lock;
    if (isEqualStr(str, "scroll lock") || isEqualStr(str, "scroll lk"))
        return Key_Scroll_Lock;

    // Misc keys.
    if (isEqualStr(str, "print screen") || isEqualStr(str, "prtsc") || isEqualStr(str, "print"))
        return Key_Print_Screen;
    if (isEqualStr(str, "menu"))                return Key_Menu;

    // OEM keys.
    if (isEqualStr(str, "`"))                   return Key_Left_Quote;
    if (isEqualStr(str, "-"))                   return Key_Minus;
    if (isEqualStr(str, "="))                   return Key_Equal;
    if (isEqualStr(str, "["))                   return Key_Left_Bracket;
    if (isEqualStr(str, "]"))                   return Key_Right_Bracket;
    if (isEqualStr(str, ";"))                   return Key_Semicolon;
    if (isEqualStr(str, "'"))                   return Key_Apostrophe;
    if (isEqualStr(str, ","))                   return Key_Comma;
    if (isEqualStr(str, "."))                   return Key_Period;
    if (isEqualStr(str, "/"))                   return Key_Slash;
    if (isEqualStr(str, "\\"))                  return Key_Backslash;
    if (isEqualStr(str, "<>"))                  return Key_Angle_Bracket;

    // Modifier keys.
    if (isEqualStr(str, "win") || isEqualStr(str, "command") ||
        isEqualStr(str, "super") || isEqualStr(str, "meta"))
        return Key_Mod_Meta;
    if (isEqualStr(str, "left win") || isEqualStr(str, "left command") ||
        isEqualStr(str, "left super") || isEqualStr(str, "left meta"))
        return Key_Mod_Meta_Left;
    if (isEqualStr(str, "right win") || isEqualStr(str, "right command") ||
        isEqualStr(str, "right super") || isEqualStr(str, "right meta"))
        return Key_Mod_Meta_Right;
    if (isEqualStr(str, "ctrl") || isEqualStr(str, "control"))
        return Key_Mod_Ctrl;
    if (isEqualStr(str, "left ctrl") || isEqualStr(str, "left control"))
        return Key_Mod_Ctrl_Left;
    if (isEqualStr(str, "right ctrl") || isEqualStr(str, "right control"))
        return Key_Mod_Ctrl_Right;
    if (isEqualStr(str, "alt") || isEqualStr(str, "option"))
        return Key_Mod_Alt;
    if (isEqualStr(str, "left alt") || isEqualStr(str, "left option"))
        return Key_Mod_Alt_Left;
    if (isEqualStr(str, "right alt") || isEqualStr(str, "right option"))
        return Key_Mod_Alt_Right;
    if (isEqualStr(str, "shift"))
        return Key_Mod_Shift;
    if (isEqualStr(str, "left shift"))
        return Key_Mod_Shift_Left;
    if (isEqualStr(str, "right shift"))
        return Key_Mod_Shift_Right;

    return Key();
}

} // namespace gbhk
