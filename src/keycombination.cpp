#include <global_hotkey/keycombination.hpp>

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

KeyCombination::KeyCombination(uint modifier, uint key, bool isAutoRepeat) :
    mod_(modifier), key_(key), isAutoRepeat_(isAutoRepeat)
{}

size_t KeyCombination::Hash::operator()(const KeyCombination& obj) const
{
    auto h1 = std::hash<uint>()(obj.mod_);
    auto h2 = std::hash<uint>()(obj.key_);

    return h1 ^ (h2 << 8);
}

uint KeyCombination::nativeModifier() const
{
    uint mod = getNativeModifier(mod_);

#if defined(GBHK_WIN)
    if (isAutoRepeat_ && mod & MOD_NOREPEAT)
        mod ^= MOD_NOREPEAT;
    else
        mod |= MOD_NOREPEAT;
#elif defined(GBHK_MAC)
// TODO
#elif defined(GBHK_LINUX)
// TODO
#endif
    return mod;
}

uint KeyCombination::modifier() const
{
    return mod_;
}

uint KeyCombination::nativeKey() const
{
    return getNativeKey(key_);
}

uint KeyCombination::key() const
{
    return key_;
}

bool KeyCombination::isAutoRepeat() const
{
    return isAutoRepeat_;
}

void KeyCombination::setModifier(uint modifier)
{
    mod_ = modifier;
}

void KeyCombination::addModifier(Modifier modifier)
{
    mod_ |= modifier;
}

void KeyCombination::removeModifier(Modifier modifier)
{
    mod_ &= ~modifier;
}

void KeyCombination::resetModifier()
{
    mod_ = 0;
}

void KeyCombination::setKey(uint key)
{
    key_ = key;
}

void KeyCombination::setIsAutoRepeat(bool isAutoRepeat)
{
    isAutoRepeat_ = isAutoRepeat;
}

void KeyCombination::reset()
{
    mod_ = 0;
    key_ = 0;
    isAutoRepeat_ = false;
}

String KeyCombination::getString(bool hasKeyId, bool hasIsAutoRepeat) const
{
    String rslt;

    if (mod_ & META) {
    #if defined(GBHK_WIN)
        rslt += GBHK_MODKEY_TEXT_WIN;
    #elif defined(GBHK_MAC)
        rslt += GBHK_MODKEY_TEXT_CMD;
    #elif defined(GBHK_LINUX)
        rslt += GBHK_MODKEY_TEXT_SUPER;
    #endif
    }

    if (mod_ & ALT) {
        if (!rslt.empty())
            rslt += " + ";
        rslt += GBHK_MODKEY_TEXT_ALT;
    }

    if (mod_ & CTRL) {
        if (!rslt.empty())
            rslt += " + ";
        rslt += GBHK_MODKEY_TEXT_CTRL;
    }

    if (mod_ & SHIFT) {
        if (!rslt.empty())
            rslt += " + ";
        rslt += GBHK_MODKEY_TEXT_SHIFT;
    }

    if (!rslt.empty())
        rslt += " + ";

    rslt += (char) key_;

    if (hasKeyId)
        rslt += " (" + std::to_string(key_) + ")";

    if (hasIsAutoRepeat)
        rslt += isAutoRepeat_ ? " [Auto Repeat]" : " [No Auto Repeat]";

    return rslt;
}

bool KeyCombination::equal(const KeyCombination& other) const
{
    return mod_ == other.mod_ && key_ == other.key_ && isAutoRepeat_ == other.isAutoRepeat_;
}

bool KeyCombination::operator==(const KeyCombination& other) const
{
    return mod_ == other.mod_ && key_ == other.key_;
}

bool KeyCombination::operator!=(const KeyCombination& other) const
{
    return !(*this == other);
}

}
