#ifndef GLOBAL_HOTKEY_REGISTER_GLOBAL_HOTKEY_HPP
#define GLOBAL_HOTKEY_REGISTER_GLOBAL_HOTKEY_HPP

#include "global_hotkey_base.hpp"

#ifndef GLOBAL_HOTKEY_NOREGISTER

#include <unordered_map>    // unordered_map

namespace gbhk
{

class GBHK_API RegGlobalHotkey final : public GlobalHotkeyBase
{
public:
    static RegGlobalHotkey& getInstance();

    int start();
    int end();
    int add(const KeyCombination& keycomb, VoidFunc callbackFunc);
    int add(const KeyCombination& keycomb, ArgFunc callbackFunc, Arg arg);
    int remove(const KeyCombination& keycomb);
    int replace(const KeyCombination& oldKeycomb, const KeyCombination& newKeycomb);

private:
    struct Task
    {
        enum OpCode
        {
            NONE,
            ADD,
            REMOVE,
            REPLACE,
            END
        };

        OpCode opc;
        union
        {
            struct
            {
                KeyCombination keycomb;
            } add;
            struct
            {
                KeyCombination keycomb;
            } remove;
            struct
            {
                KeyCombination oldKeycomb;
                KeyCombination newKeycomb;
            } replace;
        };

        Task() : opc(NONE), replace() {}
    };

    RegGlobalHotkey();
    ~RegGlobalHotkey();
    RegGlobalHotkey(const RegGlobalHotkey& other) = delete;
    RegGlobalHotkey& operator=(const RegGlobalHotkey& other) = delete;

    void work_();
    int end_();
    int add_(const KeyCombination& keycomb);
    int remove_(const KeyCombination& keycomb);
    int replace_(const KeyCombination& oldKeycomb, const KeyCombination& newKeycomb);

    void setTask_(const Task& task);
    Task getTask_();
    int waitTaskFinished_() const;

    VoidFunc getVoidFunc_(const KeyCombination& keycomb);
    ArgFuncArg getArgFuncArg_(const KeyCombination& keycomb);

    void addFunc_(const KeyCombination& keycomb, VoidFunc callbackFunc);
    void addFunc_(const KeyCombination& keycomb, ArgFunc callbackFunc, Arg arg);
    void removeFunc_(const KeyCombination& keycomb);

    Task task_;
    std::atomic<bool> hasTask_;
    std::atomic<bool> taskIsFinished_;
    std::atomic<int> taskResult_;
    std::atomic<int> keyId_;
    std::mutex mtx_;
    // Mapping of the key combination and callback functions.
    std::unordered_map<KeyCombination, VoidFunc, KeyCombination::Hash> voidFuncs_;
    std::unordered_map<KeyCombination, ArgFuncArg, KeyCombination::Hash> argFuncArgs_;
    std::unordered_map<int, KeyCombination> keyIdKeycombs_;
};

} // namespace gbhk

#endif // !GLOBAL_HOTKEY_NOREGISTER

#endif // !GLOBAL_HOTKEY_REGISTER_GLOBAL_HOTKEY_HPP
