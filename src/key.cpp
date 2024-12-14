#include <global_hotkey/key.hpp>

#include <sstream>  // stringstream

#if defined(GLOBAL_HOTKEY_WIN)
#include <Windows.h>
#elif defined(GLOBAL_HOTKEY_MAC)
#include <Carbon/Carbon.h>
#elif defined(GLOBAL_HOTKEY_LINUX)
#include <X11/Xlib.h>
#include <xcb/xcb.h>
#endif // GLOBAL_HOTKEY_WIN

namespace gbhk
{

constexpr const char* MOD_TEXT_WIN = "Win";
constexpr const char* MOD_TEXT_CMD_1 = "Command";
constexpr const char* MOD_TEXT_CMD_2 = "Cmd";
constexpr const char* MOD_TEXT_SUPER = "Super";
constexpr const char* MOD_TEXT_ALT = "Alt";
constexpr const char* MOD_TEXT_CTRL = "Ctrl";
constexpr const char* MOD_TEXT_SHIFT = "Shift";

#if defined(GLOBAL_HOTKEY_WIN)
constexpr const char* MOD_TEXT_META = MOD_TEXT_WIN;
#elif defined(GLOBAL_HOTKEY_MAC)
constexpr const char* MOD_TEXT_META = MOD_TEXT_CMD_1;
#elif defined(GLOBAL_HOTKEY_LINUX)
constexpr const char* MOD_TEXT_META = MOD_TEXT_SUPER;
#else
constexpr const char* MOD_TEXT_META = "";
#endif // GLOBAL_HOTKEY_WIN

constexpr const char* KEY_TEXT_MOUSEBUTTON_LEFT = "MouseButtonLeft";
constexpr const char* KEY_TEXT_MOUSEBUTTON_RIGHT = "MouseButtonRight";
constexpr const char* KEY_TEXT_MOUSEBUTTON_MIDDLE = "MouseButtonMiddle";
constexpr const char* KEY_TEXT_CANCEL = "Cancel";
constexpr const char* KEY_TEXT_BACKSPACE = "Backspace";
constexpr const char* KEY_TEXT_TAB = "Tab";
constexpr const char* KEY_TEXT_CLEAR = "Clear";
constexpr const char* KEY_TEXT_ENTER = "Enter";
constexpr const char* KEY_TEXT_PAUSE = "Pause";
constexpr const char* KEY_TEXT_CAPSLOCK = "CapsLock";
constexpr const char* KEY_TEXT_ESCAPE = "Escape";
constexpr const char* KEY_TEXT_SPACE = "Space";
constexpr const char* KEY_TEXT_PAGE_UP = "PageUp";
constexpr const char* KEY_TEXT_PAGE_DOWN = "PageDown";
constexpr const char* KEY_TEXT_END = "End";
constexpr const char* KEY_TEXT_HOME = "Home";
constexpr const char* KEY_TEXT_ARROW_LEFT = "Left";
constexpr const char* KEY_TEXT_ARROW_UP = "Up";
constexpr const char* KEY_TEXT_ARROW_RIGHT = "Right";
constexpr const char* KEY_TEXT_ARROW_DOWN = "Down";
constexpr const char* KEY_TEXT_SELECT = "Select";
constexpr const char* KEY_TEXT_PRINT = "Print";
constexpr const char* KEY_TEXT_EXECUTE = "Execute";
constexpr const char* KEY_TEXT_PRINTSCREEN = "PrintScreen";
constexpr const char* KEY_TEXT_INSERT = "Insert";
constexpr const char* KEY_TEXT_DELETE = "Delete";
constexpr const char* KEY_TEXT_HELP = "Help";
constexpr const char* KEY_TEXT_APPS = "Apps";
constexpr const char* KEY_TEXT_SLEEP = "Sleep";
constexpr const char* KEY_TEXT_NUMPAD0 = "Numpad0";
constexpr const char* KEY_TEXT_NUMPAD1 = "Numpad1";
constexpr const char* KEY_TEXT_NUMPAD2 = "Numpad2";
constexpr const char* KEY_TEXT_NUMPAD3 = "Numpad3";
constexpr const char* KEY_TEXT_NUMPAD4 = "Numpad4";
constexpr const char* KEY_TEXT_NUMPAD5 = "Numpad5";
constexpr const char* KEY_TEXT_NUMPAD6 = "Numpad6";
constexpr const char* KEY_TEXT_NUMPAD7 = "Numpad7";
constexpr const char* KEY_TEXT_NUMPAD8 = "Numpad8";
constexpr const char* KEY_TEXT_NUMPAD9 = "Numpad9";
constexpr const char* KEY_TEXT_MULTIPLY = "Multiply";
constexpr const char* KEY_TEXT_ADD = "Add";
constexpr const char* KEY_TEXT_SEPARATOR = "Separator";
constexpr const char* KEY_TEXT_SUBTRACT = "Subtract";
constexpr const char* KEY_TEXT_DECIMAL = "Decimal";
constexpr const char* KEY_TEXT_DIVIDE = "Divide";
constexpr const char* KEY_TEXT_F1 = "F1";
constexpr const char* KEY_TEXT_F2 = "F2";
constexpr const char* KEY_TEXT_F3 = "F3";
constexpr const char* KEY_TEXT_F4 = "F4";
constexpr const char* KEY_TEXT_F5 = "F5";
constexpr const char* KEY_TEXT_F6 = "F6";
constexpr const char* KEY_TEXT_F7 = "F7";
constexpr const char* KEY_TEXT_F8 = "F8";
constexpr const char* KEY_TEXT_F9 = "F9";
constexpr const char* KEY_TEXT_F10 = "F10";
constexpr const char* KEY_TEXT_F11 = "F11";
constexpr const char* KEY_TEXT_F12 = "F12";
constexpr const char* KEY_TEXT_F13 = "F13";
constexpr const char* KEY_TEXT_F14 = "F14";
constexpr const char* KEY_TEXT_F15 = "F15";
constexpr const char* KEY_TEXT_F16 = "F16";
constexpr const char* KEY_TEXT_F17 = "F17";
constexpr const char* KEY_TEXT_F18 = "F18";
constexpr const char* KEY_TEXT_F19 = "F19";
constexpr const char* KEY_TEXT_F20 = "F20";
constexpr const char* KEY_TEXT_F21 = "F21";
constexpr const char* KEY_TEXT_F22 = "F22";
constexpr const char* KEY_TEXT_F23 = "F23";
constexpr const char* KEY_TEXT_F24 = "F24";
constexpr const char* KEY_TEXT_NUMLOCK = "NumLock";
constexpr const char* KEY_TEXT_SCROLL_LOCK = "ScrollLock";
constexpr const char* KEY_TEXT_BROWSER_BACK = "BrowserBack";
constexpr const char* KEY_TEXT_BROWSER_FORWARD = "BrowserForward";
constexpr const char* KEY_TEXT_BROWSER_REFRESH = "BrowserRefresh";
constexpr const char* KEY_TEXT_BROWSER_STOP = "BrowserStop";
constexpr const char* KEY_TEXT_BROWSER_SEARCH = "BrowserSearch";
constexpr const char* KEY_TEXT_BROWSER_FAVORITES = "BrowserFavorites";
constexpr const char* KEY_TEXT_BROWSER_HOME = "BrowserHome";
constexpr const char* KEY_TEXT_VOLUME_MUTE = "VolumeMute";
constexpr const char* KEY_TEXT_VOLUME_UP = "VolumeUp";
constexpr const char* KEY_TEXT_VOLUME_DOWN = "VolumeDown";
constexpr const char* KEY_TEXT_MEDIA_NEXT_TRACK = "MediaNextTrack";
constexpr const char* KEY_TEXT_MEDIA_PREV_TRACK = "MediaPrevTrack";
constexpr const char* KEY_TEXT_MEDIA_STOP = "MediaStop";
constexpr const char* KEY_TEXT_MEDIA_PLAY_PAUSE = "MediaPlayPause";
constexpr const char* KEY_TEXT_LAUNCH_MAIL = "LaunchMail";
constexpr const char* KEY_TEXT_LAUNCH_MEDIA_SELECT = "LaunchMediaSelect";
constexpr const char* KEY_TEXT_LAUNCH_APP1 = "LaunchApp1";
constexpr const char* KEY_TEXT_LAUNCH_APP2 = "LaunchApp2";
constexpr const char* KEY_TEXT_ATTN = "ATTN";
constexpr const char* KEY_TEXT_CRSEL = "CRSEL";
constexpr const char* KEY_TEXT_EXSEL = "EXSEL";
constexpr const char* KEY_TEXT_ERASEEOF = "ERASEEOF";
constexpr const char* KEY_TEXT_PLAY = "Play";
constexpr const char* KEY_TEXT_ZOOM = "Zoom";
constexpr const char* KEY_TEXT_PA1 = "PA1";

}  // namespace gbhk

namespace gbhk
{

// @biref Whether the char is Space, Tab and Newline so on.
static bool isBlankChar(char ch)
{
    return ch == 0x09 || ch == 0x0A || ch == 0x0B || ch == 0x0C || ch == 0x0D || ch == 0x20;
}

// @brief Convert string to upper case and remove the blank character.
static String uniform(const String& str)
{
    String rslt;

    for (const auto& ch : str)
        if (!isBlankChar(ch))
            rslt += std::toupper(ch);

    return rslt;
}

bool isValidModifers(uint modifiers)
{
    return modifiers != 0;
}

bool isValidKey(uint key)
{
    return (key >= '0' && key <= '9') || (key >= 'A' && key <= 'Z') || (key >= KY_FIRST && key <= KY_LAST);
}

String getModifierString(Modifier modifier)
{
    switch (modifier) {
        case META:
            return MOD_TEXT_META;
        case ALT:
            return MOD_TEXT_ALT;
        case CTRL:
            return MOD_TEXT_CTRL;
        case SHIFT:
            return MOD_TEXT_SHIFT;
        default:
            return "";
    }
}

String getModifiersString(uint modifiers, char connector, bool hasSpace)
{
    String _connector = hasSpace ? (' ' + std::string(1, connector) + ' ') : std::string(1, connector);

    String rslt;

    if (modifiers & META)
        rslt += getModifierString(META);

    if (modifiers & ALT) {
        if (!rslt.empty())
            rslt += _connector;
        rslt += getModifierString(ALT);
    }

    if (modifiers & CTRL) {
        if (!rslt.empty())
            rslt += _connector;
        rslt += getModifierString(CTRL);
    }

    if (modifiers & SHIFT) {
        if (!rslt.empty())
            rslt += _connector;
        rslt += getModifierString(SHIFT);
    }

    return rslt;
}

String getKeyString(uint key)
{
    switch (key) {
        case KY_MOUSEBUTTON_LEFT:
            return KEY_TEXT_MOUSEBUTTON_LEFT;
        case KY_MOUSEBUTTON_RIGHT:
            return KEY_TEXT_MOUSEBUTTON_RIGHT;
        case KY_MOUSEBUTTON_MID:
            return KEY_TEXT_MOUSEBUTTON_MIDDLE;
        case KY_CANCEL:
            return KEY_TEXT_CANCEL;
        case KY_BACKSPACE:
            return KEY_TEXT_BACKSPACE;
        case KY_TAB:
            return KEY_TEXT_TAB;
        case KY_CLEAR:
            return KEY_TEXT_CLEAR;
        case KY_ENTER:
            return KEY_TEXT_ENTER;
        case KY_PAUSE:
            return KEY_TEXT_PAUSE;
        case KY_CAPSLOCK:
            return KEY_TEXT_CAPSLOCK;
        case KY_ESCAPE:
            return KEY_TEXT_ESCAPE;
        case KY_SPACE:
            return KEY_TEXT_SPACE;
        case KY_PAGE_UP:
            return KEY_TEXT_PAGE_UP;
        case KY_PAGE_DOWN:
            return KEY_TEXT_PAGE_DOWN;
        case KY_END:
            return KEY_TEXT_END;
        case KY_HOME:
            return KEY_TEXT_HOME;
        case KY_ARROW_LEFT:
            return KEY_TEXT_ARROW_LEFT;
        case KY_ARROW_UP:
            return KEY_TEXT_ARROW_UP;
        case KY_ARROW_RIGHT:
            return KEY_TEXT_ARROW_RIGHT;
        case KY_ARROW_DOWN:
            return KEY_TEXT_ARROW_DOWN;
        case KY_SELECT:
            return KEY_TEXT_SELECT;
        case KY_PRINT:
            return KEY_TEXT_PRINT;
        case KY_EXECUTE:
            return KEY_TEXT_EXECUTE;
        case KY_PRINTSCREEN:
            return KEY_TEXT_PRINTSCREEN;
        case KY_INSERT:
            return KEY_TEXT_INSERT;
        case KY_DELETE:
            return KEY_TEXT_DELETE;
        case KY_HELP:
            return KEY_TEXT_HELP;
        case KY_APPS:
            return KEY_TEXT_APPS;
        case KY_SLEEP:
            return KEY_TEXT_SLEEP;
        case KY_NUMPAD0:
            return KEY_TEXT_NUMPAD0;
        case KY_NUMPAD1:
            return KEY_TEXT_NUMPAD1;
        case KY_NUMPAD2:
            return KEY_TEXT_NUMPAD2;
        case KY_NUMPAD3:
            return KEY_TEXT_NUMPAD3;
        case KY_NUMPAD4:
            return KEY_TEXT_NUMPAD4;
        case KY_NUMPAD5:
            return KEY_TEXT_NUMPAD5;
        case KY_NUMPAD6:
            return KEY_TEXT_NUMPAD6;
        case KY_NUMPAD7:
            return KEY_TEXT_NUMPAD7;
        case KY_NUMPAD8:
            return KEY_TEXT_NUMPAD8;
        case KY_NUMPAD9:
            return KEY_TEXT_NUMPAD9;
        case KY_MULTIPLY:
            return KEY_TEXT_MULTIPLY;
        case KY_ADD:
            return KEY_TEXT_ADD;
        case KY_SEPARATOR:
            return KEY_TEXT_SEPARATOR;
        case KY_SUBTRACT:
            return KEY_TEXT_SUBTRACT;
        case KY_DECIMAL:
            return KEY_TEXT_DECIMAL;
        case KY_DIVIDE:
            return KEY_TEXT_DIVIDE;
        case KY_F1:
            return KEY_TEXT_F1;
        case KY_F2:
            return KEY_TEXT_F2;
        case KY_F3:
            return KEY_TEXT_F3;
        case KY_F4:
            return KEY_TEXT_F4;
        case KY_F5:
            return KEY_TEXT_F5;
        case KY_F6:
            return KEY_TEXT_F6;
        case KY_F7:
            return KEY_TEXT_F7;
        case KY_F8:
            return KEY_TEXT_F8;
        case KY_F9:
            return KEY_TEXT_F9;
        case KY_F10:
            return KEY_TEXT_F10;
        case KY_F11:
            return KEY_TEXT_F11;
        case KY_F12:
            return KEY_TEXT_F12;
        case KY_F13:
            return KEY_TEXT_F13;
        case KY_F14:
            return KEY_TEXT_F14;
        case KY_F15:
            return KEY_TEXT_F15;
        case KY_F16:
            return KEY_TEXT_F16;
        case KY_F17:
            return KEY_TEXT_F17;
        case KY_F18:
            return KEY_TEXT_F18;
        case KY_F19:
            return KEY_TEXT_F19;
        case KY_F20:
            return KEY_TEXT_F20;
        case KY_F21:
            return KEY_TEXT_F21;
        case KY_F22:
            return KEY_TEXT_F22;
        case KY_F23:
            return KEY_TEXT_F23;
        case KY_F24:
            return KEY_TEXT_F24;
        case KY_NUMLOCK:
            return KEY_TEXT_NUMLOCK;
        case KY_SCROLL_LOCK:
            return KEY_TEXT_SCROLL_LOCK;
        case KY_BROWSER_BACK:
            return KEY_TEXT_BROWSER_BACK;
        case KY_BROWSER_FORWARD:
            return KEY_TEXT_BROWSER_FORWARD;
        case KY_BROWSER_REFRESH:
            return KEY_TEXT_BROWSER_REFRESH;
        case KY_BROWSER_STOP:
            return KEY_TEXT_BROWSER_STOP;
        case KY_BROWSER_SEARCH:
            return KEY_TEXT_BROWSER_SEARCH;
        case KY_BROWSER_FAVORITES:
            return KEY_TEXT_BROWSER_FAVORITES;
        case KY_BROWSER_HOME:
            return KEY_TEXT_BROWSER_HOME;
        case KY_VOLUME_MUTE:
            return KEY_TEXT_VOLUME_MUTE;
        case KY_VOLUME_UP:
            return KEY_TEXT_VOLUME_UP;
        case KY_VOLUME_DOWN:
            return KEY_TEXT_VOLUME_DOWN;
        case KY_MEDIA_NEXT_TRACK:
            return KEY_TEXT_MEDIA_NEXT_TRACK;
        case KY_MEDIA_PREV_TRACK:
            return KEY_TEXT_MEDIA_PREV_TRACK;
        case KY_MEDIA_STOP:
            return KEY_TEXT_MEDIA_STOP;
        case KY_MEDIA_PLAY_PAUSE:
            return KEY_TEXT_MEDIA_PLAY_PAUSE;
        case KY_LAUNCH_MAIL:
            return KEY_TEXT_LAUNCH_MAIL;
        case KY_LAUNCH_MEDIA_SELECT:
            return KEY_TEXT_LAUNCH_MEDIA_SELECT;
        case KY_LAUNCH_APP1:
            return KEY_TEXT_LAUNCH_APP1;
        case KY_LAUNCH_APP2:
            return KEY_TEXT_LAUNCH_APP2;
        case KY_ATTN:
            return KEY_TEXT_ATTN;
        case KY_CRSEL:
            return KEY_TEXT_CRSEL;
        case KY_EXSEL:
            return KEY_TEXT_EXSEL;
        case KY_ERASEEOF:
            return KEY_TEXT_ERASEEOF;
        case KY_PLAY:
            return KEY_TEXT_PLAY;
        case KY_ZOOM:
            return KEY_TEXT_ZOOM;
        case KY_PA1:
            return KEY_TEXT_PA1;
        default:
            return String(1, static_cast<char>(key));
    }
}

Modifier getModifierFromString(const String& str)
{
    static const String win = uniform(MOD_TEXT_WIN);
    static const String super = uniform(MOD_TEXT_SUPER);
    static const String cmd1 = uniform(MOD_TEXT_CMD_1);
    static const String cmd2 = uniform(MOD_TEXT_CMD_2);
    static const String alt = uniform(MOD_TEXT_ALT);
    static const String ctrl = uniform(MOD_TEXT_CTRL);
    static const String shift = uniform(MOD_TEXT_SHIFT);

    String s = uniform(str);

    if (s == win || s == super || s == cmd1 || s == cmd2)
        return META;
    else if (s == alt)
        return ALT;
    else if (s == ctrl)
        return CTRL;
    else if (s == shift)
        return SHIFT;
    else
        return static_cast<Modifier>(0);
}

uint getModifiersFromString(const String& str, char connector)
{
    std::stringstream ss;
    ss << str;

    uint rslt = 0;

    String s;
    while (std::getline(ss, s, connector)) {
        Modifier mod = getModifierFromString(s);

        if (mod == 0)
            return 0;

        rslt |= mod;
    }

    return rslt;
}

uint getKeyFromString(const String& str)
{
    static const Strings keyTextTable = {
        uniform(KEY_TEXT_MOUSEBUTTON_LEFT),
        uniform(KEY_TEXT_MOUSEBUTTON_RIGHT),
        uniform(KEY_TEXT_MOUSEBUTTON_MIDDLE),
        uniform(KEY_TEXT_CANCEL),
        uniform(KEY_TEXT_BACKSPACE),
        uniform(KEY_TEXT_TAB),
        uniform(KEY_TEXT_CLEAR),
        uniform(KEY_TEXT_ENTER),
        uniform(KEY_TEXT_PAUSE),
        uniform(KEY_TEXT_CAPSLOCK),
        uniform(KEY_TEXT_ESCAPE),
        uniform(KEY_TEXT_SPACE),
        uniform(KEY_TEXT_PAGE_UP),
        uniform(KEY_TEXT_PAGE_DOWN),
        uniform(KEY_TEXT_END),
        uniform(KEY_TEXT_HOME),
        uniform(KEY_TEXT_ARROW_LEFT),
        uniform(KEY_TEXT_ARROW_UP),
        uniform(KEY_TEXT_ARROW_RIGHT),
        uniform(KEY_TEXT_ARROW_DOWN),
        uniform(KEY_TEXT_SELECT),
        uniform(KEY_TEXT_PRINT),
        uniform(KEY_TEXT_EXECUTE),
        uniform(KEY_TEXT_PRINTSCREEN),
        uniform(KEY_TEXT_INSERT),
        uniform(KEY_TEXT_DELETE),
        uniform(KEY_TEXT_HELP),
        uniform(KEY_TEXT_APPS),
        uniform(KEY_TEXT_SLEEP),
        uniform(KEY_TEXT_NUMPAD0),
        uniform(KEY_TEXT_NUMPAD1),
        uniform(KEY_TEXT_NUMPAD2),
        uniform(KEY_TEXT_NUMPAD3),
        uniform(KEY_TEXT_NUMPAD4),
        uniform(KEY_TEXT_NUMPAD5),
        uniform(KEY_TEXT_NUMPAD6),
        uniform(KEY_TEXT_NUMPAD7),
        uniform(KEY_TEXT_NUMPAD8),
        uniform(KEY_TEXT_NUMPAD9),
        uniform(KEY_TEXT_MULTIPLY),
        uniform(KEY_TEXT_ADD),
        uniform(KEY_TEXT_SEPARATOR),
        uniform(KEY_TEXT_SUBTRACT),
        uniform(KEY_TEXT_DECIMAL),
        uniform(KEY_TEXT_DIVIDE),
        uniform(KEY_TEXT_F1),
        uniform(KEY_TEXT_F2),
        uniform(KEY_TEXT_F3),
        uniform(KEY_TEXT_F4),
        uniform(KEY_TEXT_F5),
        uniform(KEY_TEXT_F6),
        uniform(KEY_TEXT_F7),
        uniform(KEY_TEXT_F8),
        uniform(KEY_TEXT_F9),
        uniform(KEY_TEXT_F10),
        uniform(KEY_TEXT_F11),
        uniform(KEY_TEXT_F12),
        uniform(KEY_TEXT_F13),
        uniform(KEY_TEXT_F14),
        uniform(KEY_TEXT_F15),
        uniform(KEY_TEXT_F16),
        uniform(KEY_TEXT_F17),
        uniform(KEY_TEXT_F18),
        uniform(KEY_TEXT_F19),
        uniform(KEY_TEXT_F20),
        uniform(KEY_TEXT_F21),
        uniform(KEY_TEXT_F22),
        uniform(KEY_TEXT_F23),
        uniform(KEY_TEXT_F24),
        uniform(KEY_TEXT_NUMLOCK),
        uniform(KEY_TEXT_SCROLL_LOCK),
        uniform(KEY_TEXT_BROWSER_BACK),
        uniform(KEY_TEXT_BROWSER_FORWARD),
        uniform(KEY_TEXT_BROWSER_REFRESH),
        uniform(KEY_TEXT_BROWSER_STOP),
        uniform(KEY_TEXT_BROWSER_SEARCH),
        uniform(KEY_TEXT_BROWSER_FAVORITES),
        uniform(KEY_TEXT_BROWSER_HOME),
        uniform(KEY_TEXT_VOLUME_MUTE),
        uniform(KEY_TEXT_VOLUME_UP),
        uniform(KEY_TEXT_VOLUME_DOWN),
        uniform(KEY_TEXT_MEDIA_NEXT_TRACK),
        uniform(KEY_TEXT_MEDIA_PREV_TRACK),
        uniform(KEY_TEXT_MEDIA_STOP),
        uniform(KEY_TEXT_MEDIA_PLAY_PAUSE),
        uniform(KEY_TEXT_LAUNCH_MAIL),
        uniform(KEY_TEXT_LAUNCH_MEDIA_SELECT),
        uniform(KEY_TEXT_LAUNCH_APP1),
        uniform(KEY_TEXT_LAUNCH_APP2),
        uniform(KEY_TEXT_ATTN),
        uniform(KEY_TEXT_CRSEL),
        uniform(KEY_TEXT_EXSEL),
        uniform(KEY_TEXT_ERASEEOF),
        uniform(KEY_TEXT_PLAY),
        uniform(KEY_TEXT_ZOOM),
        uniform(KEY_TEXT_PA1)
    };

    String s = uniform(str);

    if (s.size() == 1) {
        char ch = s[0];

        bool isDigit = ch >= '0' && ch <= '9';
        bool isAlpha = ch >= 'A' && ch <= 'Z';

        if (isDigit || isAlpha)
            return ch;
        else
            return 0;
    }

    for (uint i = 0; i < keyTextTable.size(); ++i) {
        if (s != keyTextTable[i])
            continue;

        return i + KY_FIRST;
    }

    return 0;
}

uint getNativeModifier(Modifier modifier)
{
    switch (modifier) {
        case META:
#if defined(GLOBAL_HOTKEY_WIN)
            return MOD_WIN;
#elif defined(GLOBAL_HOTKEY_MAC)
            return cmdKey;
#elif defined(GLOBAL_HOTKEY_LINUX)
            return Mod4Mask;
#endif
        case ALT:
#if defined(GLOBAL_HOTKEY_WIN)
            return MOD_ALT;
#elif defined(GLOBAL_HOTKEY_MAC)
            return optionKey;
#elif defined(GLOBAL_HOTKEY_LINUX)
            return Mod1Mask;
#endif
        case CTRL:
#if defined(GLOBAL_HOTKEY_WIN)
            return MOD_CONTROL;
#elif defined(GLOBAL_HOTKEY_MAC)
            return cmdKey;
#elif defined(GLOBAL_HOTKEY_LINUX)
            return ControlMask;
#endif
        case SHIFT:
#if defined(GLOBAL_HOTKEY_WIN)
            return MOD_SHIFT;
#elif defined(GLOBAL_HOTKEY_MAC)
            return shiftKey;
#elif defined(GLOBAL_HOTKEY_LINUX)
            return ShiftMask;
#endif
        default:
            return modifier;
    }
}

uint getNativeModifiers(uint modifiers, bool isAutoRepeat)
{
    uint mods = 0;

    if (modifiers & META)
        mods |= getNativeModifier(META);

    if (modifiers & ALT)
        mods |= getNativeModifier(ALT);

    if (modifiers & CTRL)
        mods |= getNativeModifier(CTRL);

    if (modifiers & SHIFT)
        mods |= getNativeModifier(SHIFT);

#if defined(GLOBAL_HOTKEY_WIN)
    if (isAutoRepeat)
        mods &= ~MOD_NOREPEAT;
    else
        mods |= MOD_NOREPEAT;
#elif defined(GLOBAL_HOTKEY_MAC)
// TODO
#elif defined(GLOBAL_HOTKEY_LINUX)
// TODO
#endif

    return mods;
}

uint getNativeKey(uint key)
{
    switch (key) {
        case KY_MOUSEBUTTON_LEFT:
#if defined(GLOBAL_HOTKEY_WIN)
            return VK_LBUTTON;
#elif defined(GLOBAL_HOTKEY_MAC)

#elif defined(GLOBAL_HOTKEY_LINUX)

#endif
        case KY_MOUSEBUTTON_RIGHT:
#if defined(GLOBAL_HOTKEY_WIN)
            return VK_RBUTTON;
#elif defined(GLOBAL_HOTKEY_MAC)

#elif defined(GLOBAL_HOTKEY_LINUX)

#endif
        case KY_MOUSEBUTTON_MID:
#if defined(GLOBAL_HOTKEY_WIN)
            return VK_MBUTTON;
#elif defined(GLOBAL_HOTKEY_MAC)

#elif defined(GLOBAL_HOTKEY_LINUX)

#endif
        case KY_CANCEL:
#if defined(GLOBAL_HOTKEY_WIN)
            return VK_CANCEL;
#elif defined(GLOBAL_HOTKEY_MAC)

#elif defined(GLOBAL_HOTKEY_LINUX)

#endif
        case KY_BACKSPACE:
#if defined(GLOBAL_HOTKEY_WIN)
            return VK_BACK;
#elif defined(GLOBAL_HOTKEY_MAC)

#elif defined(GLOBAL_HOTKEY_LINUX)

#endif
        case KY_TAB:
#if defined(GLOBAL_HOTKEY_WIN)
            return VK_TAB;
#elif defined(GLOBAL_HOTKEY_MAC)

#elif defined(GLOBAL_HOTKEY_LINUX)

#endif
        case KY_CLEAR:
#if defined(GLOBAL_HOTKEY_WIN)
            return VK_CLEAR;
#elif defined(GLOBAL_HOTKEY_MAC)

#elif defined(GLOBAL_HOTKEY_LINUX)

#endif
        case KY_ENTER:
#if defined(GLOBAL_HOTKEY_WIN)
            return VK_RETURN;
#elif defined(GLOBAL_HOTKEY_MAC)

#elif defined(GLOBAL_HOTKEY_LINUX)

#endif
        case KY_PAUSE:
#if defined(GLOBAL_HOTKEY_WIN)
            return VK_PAUSE;
#elif defined(GLOBAL_HOTKEY_MAC)

#elif defined(GLOBAL_HOTKEY_LINUX)

#endif
        case KY_CAPSLOCK:
#if defined(GLOBAL_HOTKEY_WIN)
            return VK_CAPITAL;
#elif defined(GLOBAL_HOTKEY_MAC)

#elif defined(GLOBAL_HOTKEY_LINUX)

#endif
        case KY_ESCAPE:
#if defined(GLOBAL_HOTKEY_WIN)
            return VK_ESCAPE;
#elif defined(GLOBAL_HOTKEY_MAC)

#elif defined(GLOBAL_HOTKEY_LINUX)

#endif
        case KY_SPACE:
#if defined(GLOBAL_HOTKEY_WIN)
            return VK_SPACE;
#elif defined(GLOBAL_HOTKEY_MAC)

#elif defined(GLOBAL_HOTKEY_LINUX)

#endif
        case KY_PAGE_UP:
#if defined(GLOBAL_HOTKEY_WIN)
            return VK_PRIOR;
#elif defined(GLOBAL_HOTKEY_MAC)

#elif defined(GLOBAL_HOTKEY_LINUX)

#endif
        case KY_PAGE_DOWN:
#if defined(GLOBAL_HOTKEY_WIN)
            return VK_NEXT;
#elif defined(GLOBAL_HOTKEY_MAC)

#elif defined(GLOBAL_HOTKEY_LINUX)

#endif
        case KY_END:
#if defined(GLOBAL_HOTKEY_WIN)
            return VK_END;
#elif defined(GLOBAL_HOTKEY_MAC)

#elif defined(GLOBAL_HOTKEY_LINUX)

#endif
        case KY_HOME:
#if defined(GLOBAL_HOTKEY_WIN)
            return VK_HOME;
#elif defined(GLOBAL_HOTKEY_MAC)

#elif defined(GLOBAL_HOTKEY_LINUX)

#endif
        case KY_ARROW_LEFT:
#if defined(GLOBAL_HOTKEY_WIN)
            return VK_LEFT;
#elif defined(GLOBAL_HOTKEY_MAC)

#elif defined(GLOBAL_HOTKEY_LINUX)

#endif
        case KY_ARROW_UP:
#if defined(GLOBAL_HOTKEY_WIN)
            return VK_UP;
#elif defined(GLOBAL_HOTKEY_MAC)

#elif defined(GLOBAL_HOTKEY_LINUX)

#endif
        case KY_ARROW_RIGHT:
#if defined(GLOBAL_HOTKEY_WIN)
            return VK_RIGHT;
#elif defined(GLOBAL_HOTKEY_MAC)

#elif defined(GLOBAL_HOTKEY_LINUX)

#endif
        case KY_ARROW_DOWN:
#if defined(GLOBAL_HOTKEY_WIN)
            return VK_DOWN;
#elif defined(GLOBAL_HOTKEY_MAC)

#elif defined(GLOBAL_HOTKEY_LINUX)

#endif
        case KY_SELECT:
#if defined(GLOBAL_HOTKEY_WIN)
            return VK_SELECT;
#elif defined(GLOBAL_HOTKEY_MAC)

#elif defined(GLOBAL_HOTKEY_LINUX)

#endif
        case KY_PRINT:
#if defined(GLOBAL_HOTKEY_WIN)
            return VK_PRINT;
#elif defined(GLOBAL_HOTKEY_MAC)

#elif defined(GLOBAL_HOTKEY_LINUX)

#endif
        case KY_EXECUTE:
#if defined(GLOBAL_HOTKEY_WIN)
            return VK_EXECUTE;
#elif defined(GLOBAL_HOTKEY_MAC)

#elif defined(GLOBAL_HOTKEY_LINUX)

#endif
        case KY_PRINTSCREEN:
#if defined(GLOBAL_HOTKEY_WIN)
            return VK_SNAPSHOT;
#elif defined(GLOBAL_HOTKEY_MAC)

#elif defined(GLOBAL_HOTKEY_LINUX)

#endif
        case KY_INSERT:
#if defined(GLOBAL_HOTKEY_WIN)
            return VK_INSERT;
#elif defined(GLOBAL_HOTKEY_MAC)

#elif defined(GLOBAL_HOTKEY_LINUX)

#endif
        case KY_DELETE:
#if defined(GLOBAL_HOTKEY_WIN)
            return VK_DELETE;
#elif defined(GLOBAL_HOTKEY_MAC)

#elif defined(GLOBAL_HOTKEY_LINUX)

#endif
        case KY_HELP:
#if defined(GLOBAL_HOTKEY_WIN)
            return VK_HELP;
#elif defined(GLOBAL_HOTKEY_MAC)

#elif defined(GLOBAL_HOTKEY_LINUX)

#endif
        case KY_APPS:
#if defined(GLOBAL_HOTKEY_WIN)
            return VK_APPS;
#elif defined(GLOBAL_HOTKEY_MAC)

#elif defined(GLOBAL_HOTKEY_LINUX)

#endif
        case KY_SLEEP:
#if defined(GLOBAL_HOTKEY_WIN)
            return VK_SLEEP;
#elif defined(GLOBAL_HOTKEY_MAC)

#elif defined(GLOBAL_HOTKEY_LINUX)

#endif
        case KY_NUMPAD0:
#if defined(GLOBAL_HOTKEY_WIN)
            return VK_NUMPAD0;
#elif defined(GLOBAL_HOTKEY_MAC)

#elif defined(GLOBAL_HOTKEY_LINUX)

#endif
        case KY_NUMPAD1:
#if defined(GLOBAL_HOTKEY_WIN)
            return VK_NUMPAD1;
#elif defined(GLOBAL_HOTKEY_MAC)

#elif defined(GLOBAL_HOTKEY_LINUX)

#endif
        case KY_NUMPAD2:
#if defined(GLOBAL_HOTKEY_WIN)
            return VK_NUMPAD2;
#elif defined(GLOBAL_HOTKEY_MAC)

#elif defined(GLOBAL_HOTKEY_LINUX)

#endif
        case KY_NUMPAD3:
#if defined(GLOBAL_HOTKEY_WIN)
            return VK_NUMPAD3;
#elif defined(GLOBAL_HOTKEY_MAC)

#elif defined(GLOBAL_HOTKEY_LINUX)

#endif
        case KY_NUMPAD4:
#if defined(GLOBAL_HOTKEY_WIN)
            return VK_NUMPAD4;
#elif defined(GLOBAL_HOTKEY_MAC)

#elif defined(GLOBAL_HOTKEY_LINUX)

#endif
        case KY_NUMPAD5:
#if defined(GLOBAL_HOTKEY_WIN)
            return VK_NUMPAD5;
#elif defined(GLOBAL_HOTKEY_MAC)

#elif defined(GLOBAL_HOTKEY_LINUX)

#endif
        case KY_NUMPAD6:
#if defined(GLOBAL_HOTKEY_WIN)
            return VK_NUMPAD6;
#elif defined(GLOBAL_HOTKEY_MAC)

#elif defined(GLOBAL_HOTKEY_LINUX)

#endif
        case KY_NUMPAD7:
#if defined(GLOBAL_HOTKEY_WIN)
            return VK_NUMPAD7;
#elif defined(GLOBAL_HOTKEY_MAC)

#elif defined(GLOBAL_HOTKEY_LINUX)

#endif
        case KY_NUMPAD8:
#if defined(GLOBAL_HOTKEY_WIN)
            return VK_NUMPAD8;
#elif defined(GLOBAL_HOTKEY_MAC)

#elif defined(GLOBAL_HOTKEY_LINUX)

#endif
        case KY_NUMPAD9:
#if defined(GLOBAL_HOTKEY_WIN)
            return VK_NUMPAD9;
#elif defined(GLOBAL_HOTKEY_MAC)

#elif defined(GLOBAL_HOTKEY_LINUX)

#endif
        case KY_MULTIPLY:
#if defined(GLOBAL_HOTKEY_WIN)
            return VK_MULTIPLY;
#elif defined(GLOBAL_HOTKEY_MAC)

#elif defined(GLOBAL_HOTKEY_LINUX)

#endif
        case KY_ADD:
#if defined(GLOBAL_HOTKEY_WIN)
            return VK_ADD;
#elif defined(GLOBAL_HOTKEY_MAC)

#elif defined(GLOBAL_HOTKEY_LINUX)

#endif
        case KY_SEPARATOR:
#if defined(GLOBAL_HOTKEY_WIN)
            return VK_SEPARATOR;
#elif defined(GLOBAL_HOTKEY_MAC)

#elif defined(GLOBAL_HOTKEY_LINUX)

#endif
        case KY_SUBTRACT:
#if defined(GLOBAL_HOTKEY_WIN)
            return VK_SUBTRACT;
#elif defined(GLOBAL_HOTKEY_MAC)

#elif defined(GLOBAL_HOTKEY_LINUX)

#endif
        case KY_DECIMAL:
#if defined(GLOBAL_HOTKEY_WIN)
            return VK_DECIMAL;
#elif defined(GLOBAL_HOTKEY_MAC)

#elif defined(GLOBAL_HOTKEY_LINUX)

#endif
        case KY_DIVIDE:
#if defined(GLOBAL_HOTKEY_WIN)
            return VK_DIVIDE;
#elif defined(GLOBAL_HOTKEY_MAC)

#elif defined(GLOBAL_HOTKEY_LINUX)

#endif
        case KY_F1:
#if defined(GLOBAL_HOTKEY_WIN)
            return VK_F1;
#elif defined(GLOBAL_HOTKEY_MAC)

#elif defined(GLOBAL_HOTKEY_LINUX)

#endif
        case KY_F2:
#if defined(GLOBAL_HOTKEY_WIN)
            return VK_F2;
#elif defined(GLOBAL_HOTKEY_MAC)

#elif defined(GLOBAL_HOTKEY_LINUX)

#endif
        case KY_F3:
#if defined(GLOBAL_HOTKEY_WIN)
            return VK_F3;
#elif defined(GLOBAL_HOTKEY_MAC)

#elif defined(GLOBAL_HOTKEY_LINUX)

#endif
        case KY_F4:
#if defined(GLOBAL_HOTKEY_WIN)
            return VK_F4;
#elif defined(GLOBAL_HOTKEY_MAC)

#elif defined(GLOBAL_HOTKEY_LINUX)

#endif
        case KY_F5:
#if defined(GLOBAL_HOTKEY_WIN)
            return VK_F5;
#elif defined(GLOBAL_HOTKEY_MAC)

#elif defined(GLOBAL_HOTKEY_LINUX)

#endif
        case KY_F6:
#if defined(GLOBAL_HOTKEY_WIN)
            return VK_F6;
#elif defined(GLOBAL_HOTKEY_MAC)

#elif defined(GLOBAL_HOTKEY_LINUX)

#endif
        case KY_F7:
#if defined(GLOBAL_HOTKEY_WIN)
            return VK_F7;
#elif defined(GLOBAL_HOTKEY_MAC)

#elif defined(GLOBAL_HOTKEY_LINUX)

#endif
        case KY_F8:
#if defined(GLOBAL_HOTKEY_WIN)
            return VK_F8;
#elif defined(GLOBAL_HOTKEY_MAC)

#elif defined(GLOBAL_HOTKEY_LINUX)

#endif
        case KY_F9:
#if defined(GLOBAL_HOTKEY_WIN)
            return VK_F9;
#elif defined(GLOBAL_HOTKEY_MAC)

#elif defined(GLOBAL_HOTKEY_LINUX)

#endif
        case KY_F10:
#if defined(GLOBAL_HOTKEY_WIN)
            return VK_F10;
#elif defined(GLOBAL_HOTKEY_MAC)

#elif defined(GLOBAL_HOTKEY_LINUX)

#endif
        case KY_F11:
#if defined(GLOBAL_HOTKEY_WIN)
            return VK_F11;
#elif defined(GLOBAL_HOTKEY_MAC)

#elif defined(GLOBAL_HOTKEY_LINUX)

#endif
        case KY_F12:
#if defined(GLOBAL_HOTKEY_WIN)
            return VK_F12;
#elif defined(GLOBAL_HOTKEY_MAC)

#elif defined(GLOBAL_HOTKEY_LINUX)

#endif
        case KY_F13:
#if defined(GLOBAL_HOTKEY_WIN)
            return VK_F13;
#elif defined(GLOBAL_HOTKEY_MAC)

#elif defined(GLOBAL_HOTKEY_LINUX)

#endif
        case KY_F14:
#if defined(GLOBAL_HOTKEY_WIN)
            return VK_F14;
#elif defined(GLOBAL_HOTKEY_MAC)

#elif defined(GLOBAL_HOTKEY_LINUX)

#endif
        case KY_F15:
#if defined(GLOBAL_HOTKEY_WIN)
            return VK_F15;
#elif defined(GLOBAL_HOTKEY_MAC)

#elif defined(GLOBAL_HOTKEY_LINUX)

#endif
        case KY_F16:
#if defined(GLOBAL_HOTKEY_WIN)
            return VK_F16;
#elif defined(GLOBAL_HOTKEY_MAC)

#elif defined(GLOBAL_HOTKEY_LINUX)

#endif
        case KY_F17:
#if defined(GLOBAL_HOTKEY_WIN)
            return VK_F17;
#elif defined(GLOBAL_HOTKEY_MAC)

#elif defined(GLOBAL_HOTKEY_LINUX)

#endif
        case KY_F18:
#if defined(GLOBAL_HOTKEY_WIN)
            return VK_F18;
#elif defined(GLOBAL_HOTKEY_MAC)

#elif defined(GLOBAL_HOTKEY_LINUX)

#endif
        case KY_F19:
#if defined(GLOBAL_HOTKEY_WIN)
            return VK_F19;
#elif defined(GLOBAL_HOTKEY_MAC)

#elif defined(GLOBAL_HOTKEY_LINUX)

#endif
        case KY_F20:
#if defined(GLOBAL_HOTKEY_WIN)
            return VK_F20;
#elif defined(GLOBAL_HOTKEY_MAC)

#elif defined(GLOBAL_HOTKEY_LINUX)

#endif
        case KY_F21:
#if defined(GLOBAL_HOTKEY_WIN)
            return VK_F21;
#elif defined(GLOBAL_HOTKEY_MAC)

#elif defined(GLOBAL_HOTKEY_LINUX)

#endif
        case KY_F22:
#if defined(GLOBAL_HOTKEY_WIN)
            return VK_F22;
#elif defined(GLOBAL_HOTKEY_MAC)

#elif defined(GLOBAL_HOTKEY_LINUX)

#endif
        case KY_F23:
#if defined(GLOBAL_HOTKEY_WIN)
            return VK_F23;
#elif defined(GLOBAL_HOTKEY_MAC)

#elif defined(GLOBAL_HOTKEY_LINUX)

#endif
        case KY_F24:
#if defined(GLOBAL_HOTKEY_WIN)
            return VK_F24;
#elif defined(GLOBAL_HOTKEY_MAC)

#elif defined(GLOBAL_HOTKEY_LINUX)

#endif
        case KY_NUMLOCK:
#if defined(GLOBAL_HOTKEY_WIN)
            return VK_NUMLOCK;
#elif defined(GLOBAL_HOTKEY_MAC)

#elif defined(GLOBAL_HOTKEY_LINUX)

#endif
        case KY_SCROLL_LOCK:
#if defined(GLOBAL_HOTKEY_WIN)
            return VK_SCROLL;
#elif defined(GLOBAL_HOTKEY_MAC)

#elif defined(GLOBAL_HOTKEY_LINUX)

#endif
        case KY_BROWSER_BACK:
#if defined(GLOBAL_HOTKEY_WIN)
            return VK_BROWSER_BACK;
#elif defined(GLOBAL_HOTKEY_MAC)

#elif defined(GLOBAL_HOTKEY_LINUX)

#endif
        case KY_BROWSER_FORWARD:
#if defined(GLOBAL_HOTKEY_WIN)
            return VK_BROWSER_FORWARD;
#elif defined(GLOBAL_HOTKEY_MAC)

#elif defined(GLOBAL_HOTKEY_LINUX)

#endif
        case KY_BROWSER_REFRESH:
#if defined(GLOBAL_HOTKEY_WIN)
            return VK_BROWSER_REFRESH;
#elif defined(GLOBAL_HOTKEY_MAC)

#elif defined(GLOBAL_HOTKEY_LINUX)

#endif
        case KY_BROWSER_STOP:
#if defined(GLOBAL_HOTKEY_WIN)
            return VK_BROWSER_STOP;
#elif defined(GLOBAL_HOTKEY_MAC)

#elif defined(GLOBAL_HOTKEY_LINUX)

#endif
        case KY_BROWSER_SEARCH:
#if defined(GLOBAL_HOTKEY_WIN)
            return VK_BROWSER_SEARCH;
#elif defined(GLOBAL_HOTKEY_MAC)

#elif defined(GLOBAL_HOTKEY_LINUX)

#endif
        case KY_BROWSER_FAVORITES:
#if defined(GLOBAL_HOTKEY_WIN)
            return VK_BROWSER_FAVORITES;
#elif defined(GLOBAL_HOTKEY_MAC)

#elif defined(GLOBAL_HOTKEY_LINUX)

#endif
        case KY_BROWSER_HOME:
#if defined(GLOBAL_HOTKEY_WIN)
            return VK_BROWSER_HOME;
#elif defined(GLOBAL_HOTKEY_MAC)

#elif defined(GLOBAL_HOTKEY_LINUX)

#endif
        case KY_VOLUME_MUTE:
#if defined(GLOBAL_HOTKEY_WIN)
            return VK_VOLUME_MUTE;
#elif defined(GLOBAL_HOTKEY_MAC)

#elif defined(GLOBAL_HOTKEY_LINUX)

#endif
        case KY_VOLUME_UP:
#if defined(GLOBAL_HOTKEY_WIN)
            return VK_VOLUME_UP;
#elif defined(GLOBAL_HOTKEY_MAC)

#elif defined(GLOBAL_HOTKEY_LINUX)

#endif
        case KY_VOLUME_DOWN:
#if defined(GLOBAL_HOTKEY_WIN)
            return VK_VOLUME_DOWN;
#elif defined(GLOBAL_HOTKEY_MAC)

#elif defined(GLOBAL_HOTKEY_LINUX)

#endif
        case KY_MEDIA_NEXT_TRACK:
#if defined(GLOBAL_HOTKEY_WIN)
            return VK_MEDIA_NEXT_TRACK;
#elif defined(GLOBAL_HOTKEY_MAC)

#elif defined(GLOBAL_HOTKEY_LINUX)

#endif
        case KY_MEDIA_PREV_TRACK:
#if defined(GLOBAL_HOTKEY_WIN)
            return VK_MEDIA_PREV_TRACK;
#elif defined(GLOBAL_HOTKEY_MAC)

#elif defined(GLOBAL_HOTKEY_LINUX)

#endif
        case KY_MEDIA_STOP:
#if defined(GLOBAL_HOTKEY_WIN)
            return VK_MEDIA_STOP;
#elif defined(GLOBAL_HOTKEY_MAC)

#elif defined(GLOBAL_HOTKEY_LINUX)

#endif
        case KY_MEDIA_PLAY_PAUSE:
#if defined(GLOBAL_HOTKEY_WIN)
            return VK_MEDIA_PLAY_PAUSE;
#elif defined(GLOBAL_HOTKEY_MAC)

#elif defined(GLOBAL_HOTKEY_LINUX)

#endif
        case KY_LAUNCH_MAIL:
#if defined(GLOBAL_HOTKEY_WIN)
            return VK_LAUNCH_MAIL;
#elif defined(GLOBAL_HOTKEY_MAC)

#elif defined(GLOBAL_HOTKEY_LINUX)

#endif
        case KY_LAUNCH_MEDIA_SELECT:
#if defined(GLOBAL_HOTKEY_WIN)
            return VK_LAUNCH_MEDIA_SELECT;
#elif defined(GLOBAL_HOTKEY_MAC)

#elif defined(GLOBAL_HOTKEY_LINUX)

#endif
        case KY_LAUNCH_APP1:
#if defined(GLOBAL_HOTKEY_WIN)
            return VK_LAUNCH_APP1;
#elif defined(GLOBAL_HOTKEY_MAC)

#elif defined(GLOBAL_HOTKEY_LINUX)

#endif
        case KY_LAUNCH_APP2:
#if defined(GLOBAL_HOTKEY_WIN)
            return VK_LAUNCH_APP2;
#elif defined(GLOBAL_HOTKEY_MAC)

#elif defined(GLOBAL_HOTKEY_LINUX)

#endif
        case KY_ATTN:
#if defined(GLOBAL_HOTKEY_WIN)
            return VK_ATTN;
#elif defined(GLOBAL_HOTKEY_MAC)

#elif defined(GLOBAL_HOTKEY_LINUX)

#endif
        case KY_CRSEL:
#if defined(GLOBAL_HOTKEY_WIN)
            return VK_CRSEL;
#elif defined(GLOBAL_HOTKEY_MAC)

#elif defined(GLOBAL_HOTKEY_LINUX)

#endif
        case KY_EXSEL:
#if defined(GLOBAL_HOTKEY_WIN)
            return VK_EXSEL;
#elif defined(GLOBAL_HOTKEY_MAC)

#elif defined(GLOBAL_HOTKEY_LINUX)

#endif
        case KY_ERASEEOF:
#if defined(GLOBAL_HOTKEY_WIN)
            return VK_EREOF;
#elif defined(GLOBAL_HOTKEY_MAC)

#elif defined(GLOBAL_HOTKEY_LINUX)

#endif
        case KY_PLAY:
#if defined(GLOBAL_HOTKEY_WIN)
            return VK_PLAY;
#elif defined(GLOBAL_HOTKEY_MAC)

#elif defined(GLOBAL_HOTKEY_LINUX)

#endif
        case KY_ZOOM:
#if defined(GLOBAL_HOTKEY_WIN)
            return VK_ZOOM;
#elif defined(GLOBAL_HOTKEY_MAC)

#elif defined(GLOBAL_HOTKEY_LINUX)

#endif
        case KY_PA1:
#if defined(GLOBAL_HOTKEY_WIN)
            return VK_PA1;
#elif defined(GLOBAL_HOTKEY_MAC)

#elif defined(GLOBAL_HOTKEY_LINUX)

#endif
        default:
            return key;
    }
}

} // namespace gbhk
