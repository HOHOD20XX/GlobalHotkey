#include <global_hotkey/key.hpp>

#ifdef _GLOBAL_HOTKEY_LINUX

#include <linux/input-event-codes.h>

namespace gbhk
{

GBHK_API Modifiers getModifiersFromNative(int nativeModifiers) noexcept
{

}

GBHK_API Key getKeyFromNative(int nativeKey) noexcept
{
    switch (nativeKey)
    {
        // Whitespace keys.
        // case
    }
}

GBHK_API int getNativeModifiers(const Modifiers& modifiers) noexcept
{

}

GBHK_API int getNativeKey(const Key& key) noexcept
{

}

} // namespace gbhk

#endif // _GLOBAL_HOTKEY_LINUX
