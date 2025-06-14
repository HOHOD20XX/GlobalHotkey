#include <functional>   // hash
#include <sstream>      // stringstream
#include <string>       // to_string

#include <global_hotkey/key_combination.hpp>

namespace gbhk
{

KeyCombination KeyCombination::fromString(const std::string& str) noexcept
{
    KeyCombination rslt;

    std::stringstream ss;
    ss << str;

    std::string s;
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

std::string KeyCombination::toString(bool isPrettySpace, bool showKeyValue) const noexcept
{
    std::string rslt;
    std::string separator = isPrettySpace ? " + " : "+";

    rslt += modifiersString(modifiers_, isPrettySpace);
    rslt += (!rslt.empty() ? separator : "") + keyString(key_);
    if (showKeyValue)
        rslt += " (" + std::to_string(key_) + ")";

    return rslt;
}

} // namespace gbhk
