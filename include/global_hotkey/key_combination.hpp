#ifndef GLOBAL_HOTKEY_KEY_COMBINATION_HPP
#define GLOBAL_HOTKEY_KEY_COMBINATION_HPP

#include <functional>   // hash
#include <string>       // string

#include "base.hpp"
#include "key.hpp"

namespace gbhk
{

class GBHK_API KeyCombination
{
public:
    constexpr inline KeyCombination() noexcept = default;
    constexpr inline KeyCombination(const Modifiers& modifiers, const Key& key) noexcept : mod(modifiers), ky(key) {}
    constexpr inline explicit KeyCombination(int64_t getCombinedValue) noexcept :
        mod((int32_t) (getCombinedValue >> 32)), ky((int32_t) getCombinedValue) {}
    explicit KeyCombination(const std::string& str) { *this = fromString(str); }

    static KeyCombination fromString(const std::string& str, char connector = '+') noexcept;
    std::string toString(char connector = '+', bool showKeyValue = false) const noexcept;

    constexpr inline Modifiers modifiers() const noexcept { return mod; }
    constexpr inline Key key() const noexcept { return ky; }
    static constexpr inline KeyCombination fromCombinedValue(int64_t value) noexcept
    { return KeyCombination((int32_t) (value >> 32), (int32_t) value); }
    constexpr inline int64_t getCombinedValue() const noexcept { return ((int64_t) mod << 32) | ((int64_t) ky << 0); }

#if _GLOBAL_HOTKEY_CPPVERS >= 201703L
    // In C++17, constexpr member functions are no longer implicitly const.
    constexpr inline void setModifiers(const Modifiers& modifiers) noexcept { mod = modifiers; }
    constexpr inline void setKey(const Key& key) noexcept { ky = key; }
#else
    inline void setModifiers(const Modifiers& modifiers) noexcept { mod = modifiers; }
    inline void setKey(const Key& key) noexcept { ky = key; }
#endif // _GLOBAL_HOTKEY_CPPVERS >= 201703L

    friend constexpr inline bool operator==(const KeyCombination& lhs, const KeyCombination& rhs) noexcept
    { return lhs.mod == rhs.mod && lhs.ky == rhs.ky; }
    friend constexpr inline bool operator!=(const KeyCombination& lhs, const KeyCombination& rhs) noexcept
    { return lhs.mod != rhs.mod || lhs.ky != rhs.ky; }

private:
    friend struct std::hash<KeyCombination>;

    Modifiers mod   = 0;
    Key ky          = 0;
};

} // namespace gbhk

namespace std
{

template <>
struct hash<gbhk::KeyCombination>
{
    size_t operator()(const gbhk::KeyCombination& obj) const noexcept
    {
        size_t h1 = std::hash<int32_t>()(obj.mod);
        size_t h2 = std::hash<int32_t>()(obj.ky);
        return h1 ^ (h2 << 1);
    }
};

} // namespace std

#endif // !GLOBAL_HOTKEY_KEY_COMBINATION_HPP
