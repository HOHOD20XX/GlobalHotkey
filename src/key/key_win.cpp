#include <windows.h>

#include <global_hotkey/key.hpp>

#ifdef _GLOBAL_HOTKEY_WIN

namespace gbhk
{

GBHK_API Modifiers getModifiersFromNative(int nativeModifiers) noexcept
{
    Modifiers rslt;
    if (nativeModifiers & MOD_WIN)
        rslt.add(MODI_WIN);
    if (nativeModifiers & MOD_CONTROL)
        rslt.add(MODI_CTRL);
    if (nativeModifiers & MOD_ALT)
        rslt.add(MODI_ALT);
    if (nativeModifiers & MOD_SHIFT)
        rslt.add(MODI_SHIFT);
    return rslt;
}

GBHK_API Key getKeyFromNative(int nativeKey) noexcept
{
    if ((nativeKey >= '0' && nativeKey <= '9') || (nativeKey >= 'A' && nativeKey <= 'Z'))
        return Key(nativeKey);

    switch (nativeKey)
    {
        // Whitespace keys.
        case VK_TAB:               return KEY_TAB;
        case VK_SPACE:             return KEY_SPACE;
        case VK_RETURN:            return KEY_ENTER;

        // Navigation keys.
        case VK_HOME:              return KEY_HOME;
        case VK_END:               return KEY_END;
        case VK_PRIOR:             return KEY_PAGE_UP;
        case VK_NEXT:              return KEY_PAGE_DOWN;
        case VK_LEFT:              return KEY_LEFT;
        case VK_UP:                return KEY_UP;
        case VK_RIGHT:             return KEY_RIGHT;
        case VK_DOWN:              return KEY_DOWN;

        // Editing keys.
        case VK_BACK:              return KEY_BACKSPACE;
        case VK_INSERT:            return KEY_INSERT;
        case VK_DELETE:            return KEY_DELETE;
        case VK_CLEAR:             return KEY_CLEAR;

        // Function keys.
        case VK_F1:                return KEY_F1;
        case VK_F2:                return KEY_F2;
        case VK_F3:                return KEY_F3;
        case VK_F4:                return KEY_F4;
        case VK_F5:                return KEY_F5;
        case VK_F6:                return KEY_F6;
        case VK_F7:                return KEY_F7;
        case VK_F8:                return KEY_F8;
        case VK_F9:                return KEY_F9;
        case VK_F10:               return KEY_F10;
        case VK_F11:               return KEY_F11;
        case VK_F12:               return KEY_F12;
        case VK_F13:               return KEY_F13;
        case VK_F14:               return KEY_F14;
        case VK_F15:               return KEY_F15;
        case VK_F16:               return KEY_F16;
        case VK_F17:               return KEY_F17;
        case VK_F18:               return KEY_F18;
        case VK_F19:               return KEY_F19;
        case VK_F20:               return KEY_F20;
        case VK_F21:               return KEY_F21;
        case VK_F22:               return KEY_F22;
        case VK_F23:               return KEY_F23;
        case VK_F24:               return KEY_F24;

        // Numpad number keys.
        case VK_NUMPAD0:           return KEY_NUMPAD_0;
        case VK_NUMPAD1:           return KEY_NUMPAD_1;
        case VK_NUMPAD2:           return KEY_NUMPAD_2;
        case VK_NUMPAD3:           return KEY_NUMPAD_3;
        case VK_NUMPAD4:           return KEY_NUMPAD_4;
        case VK_NUMPAD5:           return KEY_NUMPAD_5;
        case VK_NUMPAD6:           return KEY_NUMPAD_6;
        case VK_NUMPAD7:           return KEY_NUMPAD_7;
        case VK_NUMPAD8:           return KEY_NUMPAD_8;
        case VK_NUMPAD9:           return KEY_NUMPAD_9;

        // Numpad operator keys.
        case VK_ADD:               return KEY_NUMPAD_ADD;
        case VK_SUBTRACT:          return KEY_NUMPAD_SUBTRACT;
        case VK_MULTIPLY:          return KEY_NUMPAD_MULTIPLY;
        case VK_DIVIDE:            return KEY_NUMPAD_DIVIDE;
        case VK_DECIMAL:           return KEY_NUMPAD_DECIMAL;
        case VK_SEPARATOR:         return KEY_NUMPAD_SEPARATOR;

        // Applications keys.
        case VK_ESCAPE:            return KEY_ESCAPE;
        case VK_PAUSE:             return KEY_PAUSE;
        case VK_PLAY:              return KEY_PLAY;
        case VK_HELP:              return KEY_HELP;

        // Media keys.
        case VK_MEDIA_NEXT_TRACK:  return KEY_MEDIA_NEXT;
        case VK_MEDIA_PREV_TRACK:  return KEY_MEDIA_PREVIOUS;
        case VK_MEDIA_PLAY_PAUSE:  return KEY_MEDIA_PLAY_PAUSE;
        case VK_MEDIA_STOP:        return KEY_MEDIA_STOP;

        // Volume keys.
        case VK_VOLUME_UP:         return KEY_VOLUME_UP;
        case VK_VOLUME_DOWN:       return KEY_VOLUME_DOWN;
        case VK_VOLUME_MUTE:       return KEY_VOLUME_MUTE;

        // Lock keys.
        case VK_CAPITAL:           return KEY_CAPS_LOCK;
        case VK_NUMLOCK:           return KEY_NUM_LOCK;
        case VK_SCROLL:            return KEY_SCROLL_LOCK;

        // Misc keys.
        case VK_SNAPSHOT:          return KEY_PRINT_SCREEN;
        case VK_APPS:              return KEY_MENU;

        // OEM keys.
        case VK_OEM_3:             return KEY_LEFT_QUOTE;
        case VK_OEM_MINUS:         return KEY_MINUS;
        case VK_OEM_PLUS:          return KEY_EQUAL;
        case VK_OEM_4:             return KEY_LEFT_BRACKET;
        case VK_OEM_6:             return KEY_RIGHT_BRACKET;
        case VK_OEM_1:             return KEY_SEMICOLON;
        case VK_OEM_7:             return KEY_APOSTROPHE;
        case VK_OEM_COMMA:         return KEY_COMMA;
        case VK_OEM_PERIOD:        return KEY_PERIOD;
        case VK_OEM_2:             return KEY_SLASH;
        case VK_OEM_5:             return KEY_BACKSLASH;
        case VK_OEM_102:           return KEY_ANGLE_BRACKET;

        // Modifier keys.
        case VK_LWIN:              return KEY_MODI_META_LEFT;
        case VK_RWIN:              return KEY_MODI_META_RIGHT;
        case VK_CONTROL:           return KEY_MODI_CTRL;
        case VK_LCONTROL:          return KEY_MODI_CTRL_LEFT;
        case VK_RCONTROL:          return KEY_MODI_CTRL_RIGHT;
        case VK_MENU:              return KEY_MODI_ALT;
        case VK_LMENU:             return KEY_MODI_ALT_LEFT;
        case VK_RMENU:             return KEY_MODI_ALT_RIGHT;
        case VK_SHIFT:             return KEY_MODI_SHIFT;
        case VK_LSHIFT:            return KEY_MODI_SHIFT_LEFT;
        case VK_RSHIFT:            return KEY_MODI_SHIFT_RIGHT;

        default:                    return Key();
    }
}

GBHK_API int getNativeModifiers(const Modifiers& modifiers) noexcept
{
    int rslt = 0;
    if (modifiers.has(MODI_META))
        rslt |= MOD_WIN;
    if (modifiers.has(MODI_CTRL))
        rslt |= MOD_CONTROL;
    if (modifiers.has(MODI_ALT))
        rslt |= MOD_ALT;
    if (modifiers.has(MODI_SHIFT))
        rslt |= MOD_SHIFT;
    return rslt;
}

GBHK_API int getNativeKey(const Key& key) noexcept
{
    if ((key >= KEY_0 && key <= KEY_9) || (key >= KEY_A && key <= KEY_Z))
        return key;

    switch (key)
    {
        // Whitespace keys.
        case KEY_TAB:               return VK_TAB;
        case KEY_SPACE:             return VK_SPACE;
        case KEY_ENTER:             return VK_RETURN;

        // Navigation keys.
        case KEY_HOME:              return VK_HOME;
        case KEY_END:               return VK_END;
        case KEY_PAGE_UP:           return VK_PRIOR;
        case KEY_PAGE_DOWN:         return VK_NEXT;
        case KEY_LEFT:              return VK_LEFT;
        case KEY_UP:                return VK_UP;
        case KEY_RIGHT:             return VK_RIGHT;
        case KEY_DOWN:              return VK_DOWN;

        // Editing keys.
        case KEY_BACKSPACE:         return VK_BACK;
        case KEY_INSERT:            return VK_INSERT;
        case KEY_DELETE:            return VK_DELETE;
        case KEY_CLEAR:             return VK_CLEAR;

        // Function keys.
        case KEY_F1:                return VK_F1;
        case KEY_F2:                return VK_F2;
        case KEY_F3:                return VK_F3;
        case KEY_F4:                return VK_F4;
        case KEY_F5:                return VK_F5;
        case KEY_F6:                return VK_F6;
        case KEY_F7:                return VK_F7;
        case KEY_F8:                return VK_F8;
        case KEY_F9:                return VK_F9;
        case KEY_F10:               return VK_F10;
        case KEY_F11:               return VK_F11;
        case KEY_F12:               return VK_F12;
        case KEY_F13:               return VK_F13;
        case KEY_F14:               return VK_F14;
        case KEY_F15:               return VK_F15;
        case KEY_F16:               return VK_F16;
        case KEY_F17:               return VK_F17;
        case KEY_F18:               return VK_F18;
        case KEY_F19:               return VK_F19;
        case KEY_F20:               return VK_F20;
        case KEY_F21:               return VK_F21;
        case KEY_F22:               return VK_F22;
        case KEY_F23:               return VK_F23;
        case KEY_F24:               return VK_F24;

        // Numpad number keys.
        case KEY_NUMPAD_0:          return VK_NUMPAD0;
        case KEY_NUMPAD_1:          return VK_NUMPAD1;
        case KEY_NUMPAD_2:          return VK_NUMPAD2;
        case KEY_NUMPAD_3:          return VK_NUMPAD3;
        case KEY_NUMPAD_4:          return VK_NUMPAD4;
        case KEY_NUMPAD_5:          return VK_NUMPAD5;
        case KEY_NUMPAD_6:          return VK_NUMPAD6;
        case KEY_NUMPAD_7:          return VK_NUMPAD7;
        case KEY_NUMPAD_8:          return VK_NUMPAD8;
        case KEY_NUMPAD_9:          return VK_NUMPAD9;

        // Numpad operator keys.
        case KEY_NUMPAD_ADD:        return VK_ADD;
        case KEY_NUMPAD_SUBTRACT:   return VK_SUBTRACT;
        case KEY_NUMPAD_MULTIPLY:   return VK_MULTIPLY;
        case KEY_NUMPAD_DIVIDE:     return VK_DIVIDE;
        case KEY_NUMPAD_DECIMAL:    return VK_DECIMAL;
        case KEY_NUMPAD_SEPARATOR:  return VK_SEPARATOR;

        // Applications keys.
        case KEY_ESCAPE:            return VK_ESCAPE;
        case KEY_PAUSE:             return VK_PAUSE;
        case KEY_PLAY:              return VK_PLAY;
        case KEY_HELP:              return VK_HELP;

        // Media keys.
        case KEY_MEDIA_NEXT:        return VK_MEDIA_NEXT_TRACK;
        case KEY_MEDIA_PREVIOUS:    return VK_MEDIA_PREV_TRACK;
        case KEY_MEDIA_PLAY_PAUSE:  return VK_MEDIA_PLAY_PAUSE;
        case KEY_MEDIA_STOP:        return VK_MEDIA_STOP;

        // Volume keys.
        case KEY_VOLUME_UP:         return VK_VOLUME_UP;
        case KEY_VOLUME_DOWN:       return VK_VOLUME_DOWN;
        case KEY_VOLUME_MUTE:       return VK_VOLUME_MUTE;

        // Lock keys.
        case KEY_CAPS_LOCK:         return VK_CAPITAL;
        case KEY_NUM_LOCK:          return VK_NUMLOCK;
        case KEY_SCROLL_LOCK:       return VK_SCROLL;

        // Misc keys.
        case KEY_PRINT_SCREEN:      return VK_SNAPSHOT;
        case KEY_MENU:              return VK_APPS;

        // OEM keys.
        case KEY_LEFT_QUOTE:        return VK_OEM_3;
        case KEY_MINUS:             return VK_OEM_MINUS;
        case KEY_EQUAL:             return VK_OEM_PLUS;
        case KEY_LEFT_BRACKET:      return VK_OEM_4;
        case KEY_RIGHT_BRACKET:     return VK_OEM_6;
        case KEY_SEMICOLON:         return VK_OEM_1;
        case KEY_APOSTROPHE:        return VK_OEM_7;
        case KEY_COMMA:             return VK_OEM_COMMA;
        case KEY_PERIOD:            return VK_OEM_PERIOD;
        case KEY_SLASH:             return VK_OEM_2;
        case KEY_BACKSLASH:         return VK_OEM_5;
        case KEY_ANGLE_BRACKET:     return VK_OEM_102;

        // Modifier keys.
        case KEY_MODI_META:         return 0;    // Not supported.
        case KEY_MODI_META_LEFT:    return VK_LWIN;
        case KEY_MODI_META_RIGHT:   return VK_RWIN;
        case KEY_MODI_CTRL:         return VK_CONTROL;
        case KEY_MODI_CTRL_LEFT:    return VK_LCONTROL;
        case KEY_MODI_CTRL_RIGHT:   return VK_RCONTROL;
        case KEY_MODI_ALT:          return VK_MENU;
        case KEY_MODI_ALT_LEFT:     return VK_LMENU;
        case KEY_MODI_ALT_RIGHT:    return VK_RMENU;
        case KEY_MODI_SHIFT:        return VK_SHIFT;
        case KEY_MODI_SHIFT_LEFT:   return VK_LSHIFT;
        case KEY_MODI_SHIFT_RIGHT:  return VK_RSHIFT;

        default:                    return 0;
    }
}

} // namespace gbhk

#endif // _GLOBAL_HOTKEY_WIN
