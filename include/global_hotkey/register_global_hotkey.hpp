#ifndef GLOBAL_HOTKEY_REGISTER_GLOBAL_HOTKEY_HPP
#define GLOBAL_HOTKEY_REGISTER_GLOBAL_HOTKEY_HPP

#include "base_global_hotkey.hpp"

#include <atomic>
#include <mutex>
#include <thread>
#include <unordered_map>

namespace gbhk
{

class RegGlobalHotkey final : public BaseGlobalHotkey
{
public:
    static RegGlobalHotkey& getInstance();

    GBHK_NODISCARD uint start();
    GBHK_NODISCARD uint end();
    GBHK_NODISCARD uint add(const KeyCombination& keycomb, VoidFunc callbackFunc);
    GBHK_NODISCARD uint add(const KeyCombination& keycomb, ArgFunc callbackFunc, Arg arg);
    GBHK_NODISCARD uint remove(const KeyCombination& keycomb);
    GBHK_NODISCARD uint replace(const KeyCombination& oldKeycomb, const KeyCombination& newKeycomb);

private:
    struct Task
    {
        enum OpCode : uchar
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
    GBHK_NODISCARD uint end_();
    GBHK_NODISCARD uint add_(const KeyCombination& keycomb);
    GBHK_NODISCARD uint remove_(const KeyCombination& keycomb);
    GBHK_NODISCARD uint replace_(const KeyCombination& oldKeycomb, const KeyCombination& newKeycomb);

    void setTask_(const Task& task);
    GBHK_NODISCARD Task getTask_();
    GBHK_NODISCARD uint waitTaskFinished_() const;

    VoidFunc getVoidFunc_(const KeyCombination& keycomb);
    ArgFuncArg getArgFuncArg_(const KeyCombination& keycomb);

    void addFunc_(const KeyCombination& keycomb, VoidFunc callbackFunc);
    void addFunc_(const KeyCombination& keycomb, ArgFunc callbackFunc, Arg arg);
    void removeFunc_(const KeyCombination& keycomb);

    Task task_;
    std::atomic<bool> hasTask_;
    std::atomic<bool> taskIsFinished_;
    std::atomic<uint> taskResult_;
    std::atomic<uint> keyId_;
    std::mutex mtx_;
    // Mapping of the key combination and callback functions.
    std::unordered_map<KeyCombination, VoidFunc, KeyCombination::Hash> voidFuncs_;
    std::unordered_map<KeyCombination, ArgFuncArg, KeyCombination::Hash> argFuncArgs_;
    std::unordered_map<uint, KeyCombination> keyIdKeycombs_;
};

} // namespace gbhk

#endif // !GLOBAL_HOTKEY_REGISTER_GLOBAL_HOTKEY_HPP
