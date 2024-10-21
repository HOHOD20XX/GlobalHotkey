#include <global_hotkey/key.h>

#if defined(GBHK_WIN)
#include <Windows.h>
#elif defined(GBHK_MAC)
#include <Carbon/Carbon.h>
#elif defined(GBHK_LINUX)
#include <X11/Xlib.h>
#include <xcb/xcb.h>
#endif // GBHK_WIN

namespace gbhk
{

uint getNativeModifier(Modifier modifier)
{
    switch (modifier) {
        case META:
        #if defined(GBHK_WIN)
            return MOD_WIN;
        #elif defined(GBHK_MAC)
            return cmdKey;
        #elif defined(GBHK_LINUX)
            return Mod4Mask;
        #endif
        case ALT:
        #if defined(GBHK_WIN)
            return MOD_ALT;
        #elif defined(GBHK_MAC)
            return optionKey;
        #elif defined(GBHK_LINUX)
            return Mod1Mask;
        #endif
        case CTRL:
        #if defined(GBHK_WIN)
            return MOD_CONTROL;
        #elif defined(GBHK_MAC)
            return cmdKey;
        #elif defined(GBHK_LINUX)
            return ControlMask;
        #endif
        case SHIFT:
        #if defined(GBHK_WIN)
            return MOD_SHIFT;
        #elif defined(GBHK_MAC)
            return shiftKey;
        #elif defined(GBHK_LINUX)
            return ShiftMask;
        #endif
        default:
            return modifier;
    }
}

uint getNativeModifier(uint modifier)
{
    uint mod = 0;

    if (modifier & META) {
        mod |= getNativeModifier(META);
    }

    if (modifier & ALT) {
        mod |= getNativeModifier(ALT);
    }

    if (modifier & CTRL) {
        mod |= getNativeModifier(CTRL);
    }

    if (modifier & SHIFT) {
        mod |= getNativeModifier(SHIFT);
    }

    return mod;
}

uint getNativeKey(uint key)
{
    switch (key) {
        case KY_MOUSEBUTTON_LEFT:
        #if defined(GBHK_WIN)
            return VK_LBUTTON;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        case KY_MOUSEBUTTON_RIGHT:
        #if defined(GBHK_WIN)
            return VK_RBUTTON;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        case KY_MOUSEBUTTON_MID:
        #if defined(GBHK_WIN)
            return VK_MBUTTON;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        case KY_CANCEL:
        #if defined(GBHK_WIN)
            return VK_CANCEL;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        case KY_BACKSPACE:
        #if defined(GBHK_WIN)
            return VK_BACK;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        case KY_TAB:
        #if defined(GBHK_WIN)
            return VK_TAB;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        case KY_CLEAR:
        #if defined(GBHK_WIN)
            return VK_CLEAR;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        case KY_ENTER:
        #if defined(GBHK_WIN)
            return VK_RETURN;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        case KY_META_LEFT:
        #if defined(GBHK_WIN)
            return VK_LWIN;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        case KY_META_RIGHT:
        #if defined(GBHK_WIN)
            return VK_RWIN;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        case KY_ALT:
        #if defined(GBHK_WIN)
            return VK_MENU;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        case KY_ALT_LEFT:
        #if defined(GBHK_WIN)
            return VK_LMENU;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        case KY_ALT_RIGHT:
        #if defined(GBHK_WIN)
            return VK_RMENU;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        case KY_CTRL:
        #if defined(GBHK_WIN)
            return VK_CONTROL;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        case KY_CTRL_LEFT:
        #if defined(GBHK_WIN)
            return VK_LCONTROL;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        case KY_CTRL_RIGHT:
        #if defined(GBHK_WIN)
            return VK_RCONTROL;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        case KY_SHIFT:
        #if defined(GBHK_WIN)
            return VK_SHIFT;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        case KY_SHIFT_LEFT:
        #if defined(GBHK_WIN)
            return VK_LSHIFT;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        case KY_SHIFT_RIGHT:
        #if defined(GBHK_WIN)
            return VK_RSHIFT;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        case KY_PAUSE:
        #if defined(GBHK_WIN)
            return VK_PAUSE;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        case KY_CAPSLOCK:
        #if defined(GBHK_WIN)
            return VK_CAPITAL;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        case KY_ESCAPE:
        #if defined(GBHK_WIN)
            return VK_ESCAPE;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        case KY_SPACE:
        #if defined(GBHK_WIN)
            return VK_SPACE;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        case KY_PAGE_UP:
        #if defined(GBHK_WIN)
            return VK_PRIOR;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        case KY_PAGE_DOWN:
        #if defined(GBHK_WIN)
            return VK_NEXT;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        case KY_END:
        #if defined(GBHK_WIN)
            return VK_END;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        case KY_HOME:
        #if defined(GBHK_WIN)
            return VK_HOME;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        case KY_ARROW_LEFT:
        #if defined(GBHK_WIN)
            return VK_LEFT;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        case KY_ARROW_UP:
        #if defined(GBHK_WIN)
            return VK_UP;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        case KY_ARROW_RIGHT:
        #if defined(GBHK_WIN)
            return VK_RIGHT;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        case KY_ARROW_DOWN:
        #if defined(GBHK_WIN)
            return VK_DOWN;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        case KY_SELECT:
        #if defined(GBHK_WIN)
            return VK_SELECT;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        case KY_PRINT:
        #if defined(GBHK_WIN)
            return VK_PRINT;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        case KY_EXECUTE:
        #if defined(GBHK_WIN)
            return VK_EXECUTE;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        case KY_PRINTSCREEN:
        #if defined(GBHK_WIN)
            return VK_SNAPSHOT;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        case KY_INSERT:
        #if defined(GBHK_WIN)
            return VK_INSERT;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        case KY_DELETE:
        #if defined(GBHK_WIN)
            return VK_DELETE;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        case KY_HELP:
        #if defined(GBHK_WIN)
            return VK_HELP;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        case KY_APPS:
        #if defined(GBHK_WIN)
            return VK_APPS;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        case KY_SLEEP:
        #if defined(GBHK_WIN)
            return VK_SLEEP;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        case KY_NUMPAD0:
        #if defined(GBHK_WIN)
            return VK_NUMPAD0;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        case KY_NUMPAD1:
        #if defined(GBHK_WIN)
            return VK_NUMPAD1;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        case KY_NUMPAD2:
        #if defined(GBHK_WIN)
            return VK_NUMPAD2;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        case KY_NUMPAD3:
        #if defined(GBHK_WIN)
            return VK_NUMPAD3;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        case KY_NUMPAD4:
        #if defined(GBHK_WIN)
            return VK_NUMPAD4;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        case KY_NUMPAD5:
        #if defined(GBHK_WIN)
            return VK_NUMPAD5;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        case KY_NUMPAD6:
        #if defined(GBHK_WIN)
            return VK_NUMPAD6;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        case KY_NUMPAD7:
        #if defined(GBHK_WIN)
            return VK_NUMPAD7;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        case KY_NUMPAD8:
        #if defined(GBHK_WIN)
            return VK_NUMPAD8;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        case KY_NUMPAD9:
        #if defined(GBHK_WIN)
            return VK_NUMPAD9;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        case KY_MULTIPLY:
        #if defined(GBHK_WIN)
            return VK_MULTIPLY;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        case KY_ADD:
        #if defined(GBHK_WIN)
            return VK_ADD;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        case KY_SEPARATOR:
        #if defined(GBHK_WIN)
            return VK_SEPARATOR;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        case KY_SUBTRACT:
        #if defined(GBHK_WIN)
            return VK_SUBTRACT;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        case KY_DECIMAL:
        #if defined(GBHK_WIN)
            return VK_DECIMAL;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        case KY_DIVIDE:
        #if defined(GBHK_WIN)
            return VK_DIVIDE;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        case KY_F1:
        #if defined(GBHK_WIN)
            return VK_F1;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        case KY_F2:
        #if defined(GBHK_WIN)
            return VK_F2;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        case KY_F3:
        #if defined(GBHK_WIN)
            return VK_F3;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        case KY_F4:
        #if defined(GBHK_WIN)
            return VK_F4;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        case KY_F5:
        #if defined(GBHK_WIN)
            return VK_F5;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        case KY_F6:
        #if defined(GBHK_WIN)
            return VK_F6;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        case KY_F7:
        #if defined(GBHK_WIN)
            return VK_F7;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        case KY_F8:
        #if defined(GBHK_WIN)
            return VK_F8;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        case KY_F9:
        #if defined(GBHK_WIN)
            return VK_F9;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        case KY_F10:
        #if defined(GBHK_WIN)
            return VK_F10;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        case KY_F11:
        #if defined(GBHK_WIN)
            return VK_F11;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        case KY_F12:
        #if defined(GBHK_WIN)
            return VK_F12;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        case KY_F13:
        #if defined(GBHK_WIN)
            return VK_F13;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        case KY_F14:
        #if defined(GBHK_WIN)
            return VK_F14;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        case KY_F15:
        #if defined(GBHK_WIN)
            return VK_F15;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        case KY_F16:
        #if defined(GBHK_WIN)
            return VK_F16;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        case KY_F17:
        #if defined(GBHK_WIN)
            return VK_F17;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        case KY_F18:
        #if defined(GBHK_WIN)
            return VK_F18;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        case KY_F19:
        #if defined(GBHK_WIN)
            return VK_F19;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        case KY_F20:
        #if defined(GBHK_WIN)
            return VK_F20;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        case KY_F21:
        #if defined(GBHK_WIN)
            return VK_F21;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        case KY_F22:
        #if defined(GBHK_WIN)
            return VK_F22;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        case KY_F23:
        #if defined(GBHK_WIN)
            return VK_F23;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        case KY_F24:
        #if defined(GBHK_WIN)
            return VK_F24;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        case KY_NUMLOCK:
        #if defined(GBHK_WIN)
            return VK_NUMLOCK;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        case KY_SCROLL_LOCK:
        #if defined(GBHK_WIN)
            return VK_SCROLL;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        case KY_BROWSER_BACK:
        #if defined(GBHK_WIN)
            return VK_BROWSER_BACK;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        case KY_BROWSER_FORWARD:
        #if defined(GBHK_WIN)
            return VK_BROWSER_FORWARD;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        case KY_BROWSER_REFRESH:
        #if defined(GBHK_WIN)
            return VK_BROWSER_REFRESH;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        case KY_BROWSER_STOP:
        #if defined(GBHK_WIN)
            return VK_BROWSER_STOP;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        case KY_BROWSER_SEARCH:
        #if defined(GBHK_WIN)
            return VK_BROWSER_SEARCH;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        case KY_BROWSER_FAVORITES:
        #if defined(GBHK_WIN)
            return VK_BROWSER_FAVORITES;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        case KY_BROWSER_HOME:
        #if defined(GBHK_WIN)
            return VK_BROWSER_HOME;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        case KY_VOLUME_MUTE:
        #if defined(GBHK_WIN)
            return VK_VOLUME_MUTE;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        case KY_VOLUME_UP:
        #if defined(GBHK_WIN)
            return VK_VOLUME_UP;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        case KY_VOLUME_DOWN:
        #if defined(GBHK_WIN)
            return VK_VOLUME_DOWN;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        case KY_MEDIA_NEXT_TRACK:
        #if defined(GBHK_WIN)
            return VK_MEDIA_NEXT_TRACK;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        case KY_MEDIA_PREV_TRACK:
        #if defined(GBHK_WIN)
            return VK_MEDIA_PREV_TRACK;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        case KY_MEDIA_STOP:
        #if defined(GBHK_WIN)
            return VK_MEDIA_STOP;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        case KY_MEDIA_PLAY_PAUSE:
        #if defined(GBHK_WIN)
            return VK_MEDIA_PLAY_PAUSE;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        case KY_LAUNCH_MAIL:
        #if defined(GBHK_WIN)
            return VK_LAUNCH_MAIL;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        case KY_LAUNCH_MEDIA_SELECT:
        #if defined(GBHK_WIN)
            return VK_LAUNCH_MEDIA_SELECT;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        case KY_LAUNCH_APP1:
        #if defined(GBHK_WIN)
            return VK_LAUNCH_APP1;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        case KY_LAUNCH_APP2:
        #if defined(GBHK_WIN)
            return VK_LAUNCH_APP2;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        case KY_ATTN:
        #if defined(GBHK_WIN)
            return VK_ATTN;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        case KY_CRSEL:
        #if defined(GBHK_WIN)
            return VK_CRSEL;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        case KY_EXSEL:
        #if defined(GBHK_WIN)
            return VK_EXSEL;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        case KY_ERASEEOF:
        #if defined(GBHK_WIN)
            return VK_EREOF;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        case KY_PLAY:
        #if defined(GBHK_WIN)
            return VK_PLAY;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        case KY_ZOOM:
        #if defined(GBHK_WIN)
            return VK_ZOOM;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        case KY_PA1:
        #if defined(GBHK_WIN)
            return VK_PA1;
        #elif defined(GBHK_MAC)

        #elif defined(GBHK_LINUX)

        #endif
        default:
            return key;
    }
}

}
