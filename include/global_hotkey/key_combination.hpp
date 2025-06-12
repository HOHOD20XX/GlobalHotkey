#ifndef GLOBAL_HOTKEY_KEY_COMBINATION_HPP
#define GLOBAL_HOTKEY_KEY_COMBINATION_HPP

#include "base.hpp"
#include "key.hpp"

namespace gbhk
{

class GBHK_API KeyCombination
{
public:
    struct Hash
    {
        size_t operator()(const KeyCombination& obj) const;
    };

    constexpr inline KeyCombination() noexcept = default;
    constexpr inline KeyCombination(const Modifiers& modifiers, const Key& key) noexcept :
        modifiers_(modifiers), key_(key) {}
    inline ~KeyCombination() noexcept = default;

    static KeyCombination fromString(const String& str) noexcept;
    String toString(bool isPrettySpace = false, bool showKeyValue = false) const noexcept;

    constexpr inline Modifiers modifiers() const noexcept
    { return modifiers_; }
    constexpr inline Key key() const noexcept
    { return key_; }
    inline int nativeModifiers() const noexcept
    { return getNativeModifiers(modifiers_); }
    inline int nativeKey() const noexcept
    { return getNativeKey(key_); }

#if _GLOBAL_HOTKEY_CPPVERS >= 201703L
    // In C++17, constexpr member functions are no longer implicitly const.
    constexpr inline void setModifiers(const Modifiers& modifiers) noexcept
    { modifiers_ = modifiers; }
    constexpr inline void setKey(const Key& key) noexcept
    { key_ = key; }
#else
    inline void setModifiers(const Modifiers& modifiers) noexcept
    { modifiers_ = modifiers; }
    inline void setKey(const Key& key) noexcept
    { key_ = key; }
#endif // _GLOBAL_HOTKEY_CPPVERS >= 201703L

    constexpr inline bool isValid() const noexcept
    { return modifiers_.isValid() && key_.isValid(); }

    friend constexpr inline bool operator==(const KeyCombination& lhs, const KeyCombination& rhs) noexcept
    { return lhs.modifiers_ == rhs.modifiers_ && lhs.key_ == rhs.key_; }
    friend constexpr inline bool operator!=(const KeyCombination& lhs, const KeyCombination& rhs) noexcept
    { return lhs.modifiers_ != rhs.modifiers_ || lhs.key_ != rhs.key_; }

private:
    Modifiers modifiers_    = 0;
    Key key_                = 0;
};

} // namespace gbhk

#endif // !GLOBAL_HOTKEY_KEY_COMBINATION_HPP
