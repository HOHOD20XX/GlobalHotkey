#include <global_hotkey/keycombination.hpp>

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

KeyCombination::KeyCombination(int modifiers, int key, bool isAutoRepeat) :
    mods_(modifiers), key_(key), isAutoRepeat_(isAutoRepeat)
{}

KeyCombination KeyCombination::fromString(const String& str, char connector)
{
    KeyCombination rslt;

    std::stringstream ss;
    ss << str;

    String s;
    while (std::getline(ss, s, connector)) {
        auto mod = getModifierFromString(s);
        if (mod == 0) {
            auto key = getKeyFromString(s);

            if (key == 0)
                return rslt;
            else
                rslt.setKey(key);
        } else {
            rslt.addModifier(mod);
        }
    }

    return rslt;
}

size_t KeyCombination::Hash::operator()(const KeyCombination& obj) const
{
    auto h1 = std::hash<int>()(obj.mods_);
    auto h2 = std::hash<int>()(obj.key_);

    return h1 ^ (h2 << 8);
}

int KeyCombination::modifiers() const
{
    return mods_;
}

int KeyCombination::nativeModifiers() const
{
    return getNativeModifiers(mods_, isAutoRepeat_);
}

int KeyCombination::key() const
{
    return key_;
}

int KeyCombination::nativeKey() const
{
    return getNativeKey(key_);
}

bool KeyCombination::isAutoRepeat() const
{
    return isAutoRepeat_;
}

void KeyCombination::setModifiers(int modifier)
{
    mods_ = modifier;
}

void KeyCombination::addModifier(Modifier modifier)
{
    mods_ |= modifier;
}

void KeyCombination::removeModifier(Modifier modifier)
{
    mods_ &= ~modifier;
}

void KeyCombination::resetModifiers()
{
    mods_ = 0;
}

void KeyCombination::setKey(int key)
{
    key_ = key;
}

void KeyCombination::setIsAutoRepeat(bool isAutoRepeat)
{
    isAutoRepeat_ = isAutoRepeat;
}

void KeyCombination::reset()
{
    mods_ = 0;
    key_ = 0;
    isAutoRepeat_ = false;
}

bool KeyCombination::isValidModifers() const
{
    return gbhk::isValidModifers(mods_);
}

bool KeyCombination::isValidKey() const
{
    return gbhk::isValidKey(key_);
}

bool KeyCombination::isValid() const
{
    return isValidModifers() && isValidKey();
}

bool KeyCombination::equal(const KeyCombination& other) const
{
    return mods_ == other.mods_ && key_ == other.key_ && isAutoRepeat_ == other.isAutoRepeat_;
}

String KeyCombination::toString(char connector, bool hasSpace, bool hasKeyId, bool hasIsAutoRepeat) const
{
    String _connector = hasSpace ? (' ' + std::string(1, connector) + ' ') : std::string(1, connector);

    String rslt;

    rslt += getModifiersString(mods_, connector, hasSpace);

    if (!rslt.empty())
        rslt += _connector;

    rslt += getKeyString(key_);

    if (hasKeyId)
        rslt += " (" + std::to_string(key_) + ")";

    if (hasIsAutoRepeat)
        rslt += isAutoRepeat_ ? " [Auto Repeat]" : " [No Auto Repeat]";

    return rslt;
}

bool KeyCombination::operator==(const KeyCombination& other) const
{
    return mods_ == other.mods_ && key_ == other.key_;
}

bool KeyCombination::operator!=(const KeyCombination& other) const
{
    return !(*this == other);
}

} // namespace gbhk
