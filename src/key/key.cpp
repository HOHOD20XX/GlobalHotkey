#include <cctype>   // isspace, isalnum, toupper
#include <sstream>  // stringstream

#include <global_hotkey/key.hpp>

#define IS_SPACE(c) std::isspace(static_cast<unsigned char>(c))
#define IS_ALNUM(c) std::isalnum(static_cast<unsigned char>(c))
#define TO_UPPER(c) std::toupper(static_cast<unsigned char>(c))

namespace gbhk
{

static String modifiersStringHelper(ModifierFlag flag) noexcept
{
    switch (flag)
    {
    #ifdef _GLOBAL_HOTKEY_WIN
        case MODI_META:    return "Win";
    #elif defined(_GLOBAL_HOTKEY_MAC)
        case MODI_META:    return "Command";
    #elif defined(_GLOBAL_HOTKEY_LINUX)
        case MODI_META:    return "Super";
    #endif // _GLOBAL_HOTKEY_WIN

        case MODI_CTRL:    return "Ctrl";

    #ifdef _GLOBAL_HOTKEY_MAC
        case MODI_ALT:     return "Option";
    #else
        case MODI_ALT:     return "Alt";
    #endif // _GLOBAL_HOTKEY_MAC

        case MODI_SHIFT:   return "Shift";
        default:           return "";
    }
}

GBHK_API String modifiersString(const Modifiers& modifiers, bool isPrettySpace) noexcept
{
    String rslt;
    String separator = isPrettySpace ? " + " : "+";

    if (modifiers.has(MODI_META))
        rslt += modifiersStringHelper(MODI_META);
    if (modifiers.has(MODI_CTRL))
        rslt += (!rslt.empty() ? separator : "") + modifiersStringHelper(MODI_CTRL);
    if (modifiers.has(MODI_ALT))
        rslt += (!rslt.empty() ? separator : "") + modifiersStringHelper(MODI_ALT);
    if (modifiers.has(MODI_SHIFT))
        rslt += (!rslt.empty() ? separator : "") + modifiersStringHelper(MODI_SHIFT);

    return rslt;
}

GBHK_API String keyString(const Key& key) noexcept
{
    switch (key)
    {
        // Number keys.
        case KEY_0:                 return "0";
        case KEY_1:                 return "1";
        case KEY_2:                 return "2";
        case KEY_3:                 return "3";
        case KEY_4:                 return "4";
        case KEY_5:                 return "5";
        case KEY_6:                 return "6";
        case KEY_7:                 return "7";
        case KEY_8:                 return "8";
        case KEY_9:                 return "9";

        // Ascii alpha keys.
        case KEY_A:                 return "A";
        case KEY_B:                 return "B";
        case KEY_C:                 return "C";
        case KEY_D:                 return "D";
        case KEY_E:                 return "E";
        case KEY_F:                 return "F";
        case KEY_G:                 return "G";
        case KEY_H:                 return "H";
        case KEY_I:                 return "I";
        case KEY_J:                 return "J";
        case KEY_K:                 return "K";
        case KEY_L:                 return "L";
        case KEY_M:                 return "M";
        case KEY_N:                 return "N";
        case KEY_O:                 return "O";
        case KEY_P:                 return "P";
        case KEY_Q:                 return "Q";
        case KEY_R:                 return "R";
        case KEY_S:                 return "S";
        case KEY_T:                 return "T";
        case KEY_U:                 return "U";
        case KEY_V:                 return "V";
        case KEY_W:                 return "W";
        case KEY_X:                 return "X";
        case KEY_Y:                 return "Y";
        case KEY_Z:                 return "Z";

        // Whitespace keys.
        case KEY_TAB:               return "Tab";
        case KEY_SPACE:             return "Space";
        case KEY_ENTER:             return "Enter";

        // Navigation keys.
        case KEY_HOME:              return "Home";
        case KEY_END:               return "End";
        case KEY_PAGE_UP:           return "Page Up";
        case KEY_PAGE_DOWN:         return "Page Down";
        case KEY_LEFT:              return "Left";
        case KEY_UP:                return "Up";
        case KEY_RIGHT:             return "Right";
        case KEY_DOWN:              return "Down";

        // Editing keys.
        case KEY_BACKSPACE:         return "Backspace";
        case KEY_INSERT:            return "Insert";
        case KEY_DELETE:            return "Delete";
        case KEY_CLEAR:             return "Clear";

        // Function keys.
        case KEY_F1:                return "F1";
        case KEY_F2:                return "F2";
        case KEY_F3:                return "F3";
        case KEY_F4:                return "F4";
        case KEY_F5:                return "F5";
        case KEY_F6:                return "F6";
        case KEY_F7:                return "F7";
        case KEY_F8:                return "F8";
        case KEY_F9:                return "F9";
        case KEY_F10:               return "F10";
        case KEY_F11:               return "F11";
        case KEY_F12:               return "F12";
        case KEY_F13:               return "F13";
        case KEY_F14:               return "F14";
        case KEY_F15:               return "F15";
        case KEY_F16:               return "F16";
        case KEY_F17:               return "F17";
        case KEY_F18:               return "F18";
        case KEY_F19:               return "F19";
        case KEY_F20:               return "F20";
        case KEY_F21:               return "F21";
        case KEY_F22:               return "F22";
        case KEY_F23:               return "F23";
        case KEY_F24:               return "F24";

        // Numpad number keys.
        case KEY_NUMPAD_0:          return "Numpad 0";
        case KEY_NUMPAD_1:          return "Numpad 1";
        case KEY_NUMPAD_2:          return "Numpad 2";
        case KEY_NUMPAD_3:          return "Numpad 3";
        case KEY_NUMPAD_4:          return "Numpad 4";
        case KEY_NUMPAD_5:          return "Numpad 5";
        case KEY_NUMPAD_6:          return "Numpad 6";
        case KEY_NUMPAD_7:          return "Numpad 7";
        case KEY_NUMPAD_8:          return "Numpad 8";
        case KEY_NUMPAD_9:          return "Numpad 9";

        // Numpad operator keys.
        case KEY_NUMPAD_ADD:        return "Numpad Add";
        case KEY_NUMPAD_SUBTRACT:   return "Numpad Subtract";
        case KEY_NUMPAD_MULTIPLY:   return "Numpad Multiply";
        case KEY_NUMPAD_DIVIDE:     return "Numpad Divide";
        case KEY_NUMPAD_DECIMAL:    return "Numpad Decimal";
        case KEY_NUMPAD_SEPARATOR:  return "Numpad Separator";

        // Applications keys.
        case KEY_ESCAPE:            return "Esc";
        case KEY_PAUSE:             return "Pause";
        case KEY_PLAY:              return "Play";
        case KEY_HELP:              return "Help";

        // Media keys.
        case KEY_MEDIA_NEXT:        return "Media Next";
        case KEY_MEDIA_PREVIOUS:    return "Media Previous";
        case KEY_MEDIA_PLAY_PAUSE:  return "Media Play/Pause";
        case KEY_MEDIA_STOP:        return "Media Stop";

        // Volume keys.
        case KEY_VOLUME_UP:         return "Volume Up";
        case KEY_VOLUME_DOWN:       return "Volume Down";
        case KEY_VOLUME_MUTE:       return "Volume Mute";

        // Lock keys.
        case KEY_CAPS_LOCK:         return "CapsLock";
        case KEY_NUM_LOCK:          return "NumLock";
        case KEY_SCROLL_LOCK:       return "ScrollLock";

        // Misc keys.
        case KEY_PRINT_SCREEN:      return "PrintScreen";
        case KEY_MENU:              return "Menu";

        // OEM keys.
        case KEY_LEFT_QUOTE:        return "`";
        case KEY_MINUS:             return "-";
        case KEY_EQUAL:             return "=";
        case KEY_LEFT_BRACKET:      return "[";
        case KEY_RIGHT_BRACKET:     return "]";
        case KEY_SEMICOLON:         return ";";
        case KEY_APOSTROPHE:        return "'";
        case KEY_COMMA:             return ",";
        case KEY_PERIOD:            return ".";
        case KEY_SLASH:             return "/";
        case KEY_BACKSLASH:         return "\\";
        case KEY_ANGLE_BRACKET:     return "<>";

        default:                     return "";
    }
}

// Check if two strings are equal, ignoring case and spaces and underscores.
static bool isEqualStr(const String& str1, const String& str2) noexcept
{
    int i = 0, j = 0;
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

static int getModifiersFromStringHelper(const String& str) noexcept
{
    if (isEqualStr(str, "win") || isEqualStr(str, "command") || isEqualStr(str, "super"))
        return MODI_META;
    if (isEqualStr(str, "ctrl") || isEqualStr(str, "control"))
        return MODI_CTRL;
    if (isEqualStr(str, "alt") || isEqualStr(str, "option"))
        return MODI_ALT;
    if (isEqualStr(str, "shift"))
        return MODI_SHIFT;
    return 0;
}

GBHK_API Modifiers getModifiersFromString(const String& str) noexcept
{
    std::stringstream ss;
    ss << str;
    Modifiers rslt;
    String s;
    while (std::getline(ss, s, '+'))
        rslt.add(getModifiersFromStringHelper(s));
    return rslt;
}

GBHK_API Key getKeyFromString(const String& str) noexcept
{
    if (str.empty())
        return Key();

    if (str.size() == 1 && IS_ALNUM(str[0]))
        return Key(str[0]);

    // Whitespace keys.
    if (isEqualStr(str, "tab"))                 return KEY_TAB;
    if (isEqualStr(str, "space"))               return KEY_SPACE;
    if (isEqualStr(str, "enter") || isEqualStr(str, "return"))
        return KEY_ENTER;

    // Navigation keys.
    if (isEqualStr(str, "home"))                return KEY_HOME;
    if (isEqualStr(str, "end"))                 return KEY_END;
    if (isEqualStr(str, "page up") || isEqualStr(str, "pgup"))
        return KEY_PAGE_UP;
    if (isEqualStr(str, "page down") || isEqualStr(str, "pgdn"))
        return KEY_PAGE_DOWN;
    if (isEqualStr(str, "left") || isEqualStr(str, "arrow left"))
        return KEY_LEFT;
    if (isEqualStr(str, "up") || isEqualStr(str, "arrow up"))
        return KEY_UP;
    if (isEqualStr(str, "right") || isEqualStr(str, "arrow right"))
        return KEY_RIGHT;
    if (isEqualStr(str, "down") || isEqualStr(str, "arrow down"))
        return KEY_DOWN;

    // Editing keys.
    if (isEqualStr(str, "backspace") || isEqualStr(str, "back"))
        return KEY_BACKSPACE;
    if (isEqualStr(str, "insert") || isEqualStr(str, "ins"))
        return KEY_INSERT;
    if (isEqualStr(str, "delete") || isEqualStr(str, "del"))
        return KEY_DELETE;
    if (isEqualStr(str, "clear"))               return KEY_CLEAR;

    // Function keys.
    if (isEqualStr(str, "f1"))                  return KEY_F1;
    if (isEqualStr(str, "f2"))                  return KEY_F2;
    if (isEqualStr(str, "f3"))                  return KEY_F3;
    if (isEqualStr(str, "f4"))                  return KEY_F4;
    if (isEqualStr(str, "f5"))                  return KEY_F5;
    if (isEqualStr(str, "f6"))                  return KEY_F6;
    if (isEqualStr(str, "f7"))                  return KEY_F7;
    if (isEqualStr(str, "f8"))                  return KEY_F8;
    if (isEqualStr(str, "f9"))                  return KEY_F9;
    if (isEqualStr(str, "f10"))                 return KEY_F10;
    if (isEqualStr(str, "f11"))                 return KEY_F11;
    if (isEqualStr(str, "f12"))                 return KEY_F12;
    if (isEqualStr(str, "f13"))                 return KEY_F13;
    if (isEqualStr(str, "f14"))                 return KEY_F14;
    if (isEqualStr(str, "f15"))                 return KEY_F15;
    if (isEqualStr(str, "f16"))                 return KEY_F16;
    if (isEqualStr(str, "f17"))                 return KEY_F17;
    if (isEqualStr(str, "f18"))                 return KEY_F18;
    if (isEqualStr(str, "f19"))                 return KEY_F19;
    if (isEqualStr(str, "f20"))                 return KEY_F20;
    if (isEqualStr(str, "f21"))                 return KEY_F21;
    if (isEqualStr(str, "f22"))                 return KEY_F22;
    if (isEqualStr(str, "f23"))                 return KEY_F23;
    if (isEqualStr(str, "f24"))                 return KEY_F24;

    // Numpad number keys.
    if (isEqualStr(str, "numpad 0"))            return KEY_NUMPAD_0;
    if (isEqualStr(str, "numpad 1"))            return KEY_NUMPAD_1;
    if (isEqualStr(str, "numpad 2"))            return KEY_NUMPAD_2;
    if (isEqualStr(str, "numpad 3"))            return KEY_NUMPAD_3;
    if (isEqualStr(str, "numpad 4"))            return KEY_NUMPAD_4;
    if (isEqualStr(str, "numpad 5"))            return KEY_NUMPAD_5;
    if (isEqualStr(str, "numpad 6"))            return KEY_NUMPAD_6;
    if (isEqualStr(str, "numpad 7"))            return KEY_NUMPAD_7;
    if (isEqualStr(str, "numpad 8"))            return KEY_NUMPAD_8;
    if (isEqualStr(str, "numpad 9"))            return KEY_NUMPAD_9;

    // Numpad operator keys.
    if (isEqualStr(str, "numpad add"))          return KEY_NUMPAD_ADD;
    if (isEqualStr(str, "numpad subtract"))     return KEY_NUMPAD_SUBTRACT;
    if (isEqualStr(str, "numpad multiply"))     return KEY_NUMPAD_MULTIPLY;
    if (isEqualStr(str, "numpad divide"))       return KEY_NUMPAD_DIVIDE;
    if (isEqualStr(str, "numpad decimal"))      return KEY_NUMPAD_DECIMAL;
    if (isEqualStr(str, "numpad separator"))    return KEY_NUMPAD_SEPARATOR;

    // Applications keys.
    if (isEqualStr(str, "esc") || isEqualStr(str, "escape"))
        return KEY_ESCAPE;
    if (isEqualStr(str, "pause"))               return KEY_PAUSE;
    if (isEqualStr(str, "play"))                return KEY_PLAY;
    if (isEqualStr(str, "help"))                return KEY_HELP;

    // Media keys.
    if (isEqualStr(str, "media next") || isEqualStr(str, "media track next"))
        return KEY_MEDIA_NEXT;
    if (isEqualStr(str, "media previous") || isEqualStr(str, "media prev") ||
        isEqualStr(str, "media track previous") || isEqualStr(str, "media track prev"))
        return KEY_MEDIA_PREVIOUS;
    if (isEqualStr(str, "media play/pause") || isEqualStr(str, "media play pause"))
        return KEY_MEDIA_PLAY_PAUSE;
    if (isEqualStr(str, "media stop"))          return KEY_MEDIA_STOP;

    // Volume keys.
    if (isEqualStr(str, "volume up") || isEqualStr(str, "vol up"))
        return KEY_VOLUME_UP;
    if (isEqualStr(str, "volume down") || isEqualStr(str, "vol down"))
        return KEY_VOLUME_DOWN;
    if (isEqualStr(str, "volume mute") || isEqualStr(str, "vol mute"))
        return KEY_VOLUME_MUTE;

    // Lock keys.
    if (isEqualStr(str, "caps lock") || isEqualStr(str, "caps lk"))
        return KEY_CAPS_LOCK;
    if (isEqualStr(str, "num lock") || isEqualStr(str, "num lk"))
        return KEY_NUM_LOCK;
    if (isEqualStr(str, "scroll lock") || isEqualStr(str, "scroll lk"))
        return KEY_SCROLL_LOCK;

    // Misc keys.
    if (isEqualStr(str, "print screen") || isEqualStr(str, "prtsc"))
        return KEY_PRINT_SCREEN;
    if (isEqualStr(str, "menu"))                return KEY_MENU;

    // OEM keys.
    if (isEqualStr(str, "`"))                   return KEY_LEFT_QUOTE;
    if (isEqualStr(str, "-"))                   return KEY_MINUS;
    if (isEqualStr(str, "="))                   return KEY_EQUAL;
    if (isEqualStr(str, "["))                   return KEY_LEFT_BRACKET;
    if (isEqualStr(str, "]"))                   return KEY_RIGHT_BRACKET;
    if (isEqualStr(str, ";"))                   return KEY_SEMICOLON;
    if (isEqualStr(str, "'"))                   return KEY_APOSTROPHE;
    if (isEqualStr(str, ","))                   return KEY_COMMA;
    if (isEqualStr(str, "."))                   return KEY_PERIOD;
    if (isEqualStr(str, "/"))                   return KEY_SLASH;
    if (isEqualStr(str, "\\"))                  return KEY_BACKSLASH;
    if (isEqualStr(str, "<>"))                  return KEY_ANGLE_BRACKET;

    return Key();
}

} // namespace gbhk
