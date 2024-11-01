#ifndef KEY_HPP
#define KEY_HPP

#include <string>

#include "core/core.hpp"

namespace gbhk
{

enum Modifier : uint
{
    META = 0X01,
    ALT = 0X02,
    CTRL = 0X04,
    SHIFT = 0X08
};

enum Key : uint
{
    KY_MOUSEBUTTON_LEFT = 0X100,
    KY_MOUSEBUTTON_RIGHT,
    KY_MOUSEBUTTON_MID,
    KY_CANCEL,
    KY_BACKSPACE,
    KY_TAB,
    KY_CLEAR,
    KY_ENTER,
    //KY_META_LEFT,
    //KY_META_RIGHT,
    //KY_ALT,
    //KY_ALT_LEFT,
    //KY_ALT_RIGHT,
    //KY_CTRL,
    //KY_CTRL_LEFT,
    //KY_CTRL_RIGHT,
    //KY_SHIFT,
    //KY_SHIFT_LEFT,
    //KY_SHIFT_RIGHT,
    KY_PAUSE,
    KY_CAPSLOCK,
    KY_ESCAPE,
    KY_SPACE,
    KY_PAGE_UP,
    KY_PAGE_DOWN,
    KY_END,
    KY_HOME,
    KY_ARROW_LEFT,
    KY_ARROW_UP,
    KY_ARROW_RIGHT,
    KY_ARROW_DOWN,
    KY_SELECT,
    KY_PRINT,
    KY_EXECUTE,
    KY_PRINTSCREEN,
    KY_INSERT,
    KY_DELETE,
    KY_HELP,
    KY_APPS,
    KY_SLEEP,
    KY_NUMPAD0,
    KY_NUMPAD1,
    KY_NUMPAD2,
    KY_NUMPAD3,
    KY_NUMPAD4,
    KY_NUMPAD5,
    KY_NUMPAD6,
    KY_NUMPAD7,
    KY_NUMPAD8,
    KY_NUMPAD9,
    KY_MULTIPLY,
    KY_ADD,
    KY_SEPARATOR,
    KY_SUBTRACT,
    KY_DECIMAL,
    KY_DIVIDE,
    KY_F1,
    KY_F2,
    KY_F3,
    KY_F4,
    KY_F5,
    KY_F6,
    KY_F7,
    KY_F8,
    KY_F9,
    KY_F10,
    KY_F11,
    KY_F12,
    KY_F13,
    KY_F14,
    KY_F15,
    KY_F16,
    KY_F17,
    KY_F18,
    KY_F19,
    KY_F20,
    KY_F21,
    KY_F22,
    KY_F23,
    KY_F24,
    KY_NUMLOCK,
    KY_SCROLL_LOCK,
    KY_BROWSER_BACK,
    KY_BROWSER_FORWARD,
    KY_BROWSER_REFRESH,
    KY_BROWSER_STOP,
    KY_BROWSER_SEARCH,
    KY_BROWSER_FAVORITES,
    KY_BROWSER_HOME,
    KY_VOLUME_MUTE,
    KY_VOLUME_UP,
    KY_VOLUME_DOWN,
    KY_MEDIA_NEXT_TRACK,
    KY_MEDIA_PREV_TRACK,
    KY_MEDIA_STOP,
    KY_MEDIA_PLAY_PAUSE,
    KY_LAUNCH_MAIL,
    KY_LAUNCH_MEDIA_SELECT,
    KY_LAUNCH_APP1,
    KY_LAUNCH_APP2,
    KY_ATTN,
    KY_CRSEL,
    KY_EXSEL,
    KY_ERASEEOF,
    KY_PLAY,
    KY_ZOOM,
    KY_PA1
};

std::string getModifierString(Modifier modifier);

std::string getModifierString(uint modifier);

std::string getKeyString(uint key);

uint getNativeModifier(Modifier modifier);

uint getNativeModifier(uint modifier);

uint getNativeKey(uint key);

}

#endif // !KEY_HPP
