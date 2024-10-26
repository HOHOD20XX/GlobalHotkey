#ifndef KEYCOMBINATION_HPP
#define KEYCOMBINATION_HPP

#include <string>
#include <functional>   // hash

#include "core/core.hpp"
#include "key.hpp"

namespace gbhk
{

class KeyCombination
{
public:
    struct Hash
    {
        size_t operator()(const KeyCombination& obj) const;
    };

    KeyCombination() = default;

    KeyCombination(uint modifier, uint key, bool isAutoRepeat = false);

    uint nativeModifier() const;

    uint modifier() const;

    uint nativeKey() const;

    uint key() const;

    bool isAutoRepeat() const;

    void setModifier(uint modifier);

    void addModifier(Modifier modifier);

    void removeModifier(Modifier modifier);

    void resetModifier();

    void setKey(uint key);

    void setIsAutoRepeat(bool isAutoRepeat);

    void reset();

    // Ease to debug.
    String getString(bool hasKeyId = true, bool hasIsAutoRepeat = true) const;

    // @attention Compare the #isAutoRepeat.
    bool equal(const KeyCombination& other) const;

    // @attention Not compare the #isAutoRepeat.
    bool operator==(const KeyCombination& other) const;

    // @attention Not compare the #isAutoRepeat.
    bool operator!=(const KeyCombination& other) const;

private:
    uint mod_ = 0;
    uint key_ = 0;
    bool isAutoRepeat_ = false;
};

}

#endif // !KEYCOMBINATION_HPP
