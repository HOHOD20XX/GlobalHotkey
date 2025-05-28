#ifndef GLOBAL_HOTKEY_KEYCOMBINATION_HPP
#define GLOBAL_HOTKEY_KEYCOMBINATION_HPP

#include <functional>   // hash

#include "core/base.hpp"
#include "key.hpp"

namespace gbhk
{

class GBHK_API KeyCombination
{
public:
    /// @attention Not hash the #isAutoRepeat.
    struct Hash
    {
        size_t operator()(const KeyCombination& obj) const;
    };

    KeyCombination() = default;
    KeyCombination(int modifiers, int key, bool isAutoRepeat = false);

    static KeyCombination fromString(const String& str);

    int modifiers() const;
    int nativeModifiers() const;
    int key() const;
    int nativeKey() const;
    bool isAutoRepeat() const;

    void setModifiers(int modifiers);
    void addModifier(Modifier modifier);
    void removeModifier(Modifier modifier);
    void resetModifiers();
    void setKey(int key);
    void setIsAutoRepeat(bool isAutoRepeat);

    /// @brief #modifiers and #key be set to 0, and #isAutoRepeat be set to false.
    void reset();

    /// @brief Check whether the modifiers contain at least one modifier.
    bool isValidModifiers() const;

    /// @brief Check whether the key is valid.
    /// @return True if the key is valid keyboard value else False.
    bool isValidKey() const;

    /// @brief Check whether the key combination contain at least one modifier and a valid key.
    bool isValid() const;

    /// @param hasIsAutoRepeat  Whether the result should be attach info about if the key combination auto repeat.
    /// @param hasKeyId         Whether the result should be attach the key number value.
    String toString(bool isPrettySpace = false, bool hasIsAutoRepeat = false, bool hasKeyId = false) const;

    /// @attention Not compare the #isAutoRepeat.
    bool operator==(const KeyCombination& other) const;

    /// @attention Not compare the #isAutoRepeat.
    bool operator!=(const KeyCombination& other) const;

private:
    int mods_           = 0;
    int key_            = 0;
    bool isAutoRepeat_  = false;
};

} // namespace gbhk

#endif // !GLOBAL_HOTKEY_KEYCOMBINATION_HPP
