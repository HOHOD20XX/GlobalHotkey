#include <sstream>  // stringstream
#include <string>   // to_string
#include <vector>   // vector

#include <global_hotkey/key_combination.hpp>

namespace gbhk
{

static std::vector<std::string> splitString(const std::string& str, char separator)
{
    std::stringstream ss;
    ss << str;
    std::vector<std::string> rslt;
    std::string s;
    while (std::getline(ss, s, separator))
        rslt.emplace_back(s);
    return rslt;
}

KeyCombination KeyCombination::fromString(const std::string& str, char connector) noexcept
{
    size_t pos = str.rfind(connector);
    if (pos == std::string::npos)
        return {};
    Modifiers mod = getModifiersFromString(str.substr(0, pos));
    Key key = getKeyFromString(str.substr(pos));
    return {mod, key};
}

std::string KeyCombination::toString(char connector, bool showKeyValue) const noexcept
{
    std::string rslt;
    std::string connectorStr(1, connector);
    rslt += modifiersString(mod, connector);
    rslt += (!rslt.empty() ? connectorStr : "") + keyString(ky);
    if (showKeyValue)
        rslt += "(" + std::to_string(ky) + ")";
    return rslt;
}

} // namespace gbhk
