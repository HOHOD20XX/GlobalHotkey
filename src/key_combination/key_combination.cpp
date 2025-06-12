#include <functional>   // hash
#include <sstream>      // stringstream
#include <string>       // to_string

#include <global_hotkey/key_combination.hpp>

namespace gbhk
{

size_t KeyCombination::Hash::operator()(const KeyCombination& obj) const
{
    size_t h1 = std::hash<int>()(obj.modifiers_);
    size_t h2 = std::hash<int>()(obj.key_);
    return h1 ^ (h2 << 1);
}

KeyCombination KeyCombination::fromString(const String& str) noexcept
{
    KeyCombination rslt;

    std::stringstream ss;
    ss << str;

    String s;
    while (std::getline(ss, s, '+'))
    {
        auto modi = getModifiersFromString(s);
        if (!modi.isValid())
        {
            auto key = getKeyFromString(s);
            if (key.isValid())
                rslt.key_ = key;
            else
                return rslt;
        }
        else
        {
            rslt.modifiers_.add(modi);
        }
    }

    return rslt;
}

String KeyCombination::toString(bool isPrettySpace, bool showKeyValue) const noexcept
{
    String rslt;
    String separator = isPrettySpace ? " + " : "+";

    rslt += modifiersString(modifiers_, isPrettySpace);
    rslt += (!rslt.empty() ? separator : "") + keyString(key_);
    if (showKeyValue)
        rslt += " (" + std::to_string(key_) + ")";

    return rslt;
}

} // namespace gbhk
