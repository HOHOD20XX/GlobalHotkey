#ifndef GLOBAL_HOTKEY_GLOBAL_HOTKEY_MANAGER_HPP
#define GLOBAL_HOTKEY_GLOBAL_HOTKEY_MANAGER_HPP

#include "key_combination.hpp"

namespace gbhk
{

class _GlobalHotkeyManagerPrivate;

class GBHK_API GlobalHotkeyManager
{
public:
    GlobalHotkeyManager(const GlobalHotkeyManager&) = delete;
    GlobalHotkeyManager& operator=(const GlobalHotkeyManager&) = delete;

    /// @brief Start the global hotkey manager.
    /// @return A #ReturnCode or a platfrom-specific error code.
    virtual int start() = 0;
    /// @brief End the global hotkey manager.
    /// @return A #ReturnCode or a platfrom-specific error code.
    /// @note All hotkeys will be removed.
    virtual int end() = 0;
    /// @brief Add a hotkey to the global hotkey manager.
    /// @param autoRepeat Whether the hotkey should be auto-repeated when held down.
    /// @return A #ReturnCode or a platfrom-specific error code.
    virtual int add(const KeyCombination& kc, VoidFunc func, bool autoRepeat = false) = 0;
    /// @overload
    virtual int add(const KeyCombination& kc, ArgFunc func, Arg arg, bool autoRepeat = false) = 0;
    /// @brief Remove a hotkey from the global hotkey manager.
    /// @return A #ReturnCode or a platfrom-specific error code.
    virtual int remove(const KeyCombination& kc) = 0;
    /// @brief Remove all hotkeys in the global hotkey manager.
    /// @return A #ReturnCode or a platfrom-specific error code.
    virtual int removeAll() = 0;
    /// @brief Replace a hotkey with another one.
    /// @return A #ReturnCode or a platfrom-specific error code.
    virtual int replace(const KeyCombination& oldKc, const KeyCombination& newKc) = 0;
    /// @brief Set whether a hotkey should be auto-repeated when held down.
    /// @return A #ReturnCode or a platfrom-specific error code.
    virtual int setAutoRepeat(const KeyCombination& kc, bool autoRepeat) = 0;
    /// @return Always #RC_SUCCESS.
    virtual int setCycleTime(size_t milliseconds) = 0;
    virtual bool has(const KeyCombination& kc) const = 0;
    virtual bool isAutoRepeat(const KeyCombination& kc) const = 0;
    virtual bool isRunning() const = 0;

protected:
    explicit GlobalHotkeyManager(_GlobalHotkeyManagerPrivate* p);
    virtual ~GlobalHotkeyManager();

    _GlobalHotkeyManagerPrivate* p_ = nullptr;
};

} // namespace gbhk

#endif // !GLOBAL_HOTKEY_GLOBAL_HOTKEY_MANAGER_HPP
