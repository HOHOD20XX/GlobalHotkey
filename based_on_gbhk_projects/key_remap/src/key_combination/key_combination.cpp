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
                rslt.ky = key;
            else
                return rslt;
        }
        else
        {
            rslt.mod.add(modi);
        }
    }

    return rslt;
}

std::string KeyCombination::toString(bool isPrettySpace, bool showKeyValue) const noexcept
{
    std::string rslt;
    std::string separator = isPrettySpace ? " + " : "+";

    rslt += modifiersString(mod, isPrettySpace);
    rslt += (!rslt.empty() ? separator : "") + keyString(ky);
    if (showKeyValue)
        rslt += " (" + std::to_string(ky) + ")";

    return rslt;
}

} // namespace gbhk
