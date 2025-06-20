#ifndef GLOBAL_HOTKEY_REGISTER_GHM_PRIVATE_HPP
#define GLOBAL_HOTKEY_REGISTER_GHM_PRIVATE_HPP

#ifndef GLOBAL_HOTKEY_DISABLE_REGISTER

#include "../global_hotkey_manager/ghm_private.hpp"

namespace gbhk
{

class _RegisterGHMPrivate : public _GHMPrivate
{
public:
    _RegisterGHMPrivate();
    ~_RegisterGHMPrivate();

protected:
    int specializedStart() override final;
    int specializedEnd() override final;
    int specializedAdd(const KeyCombination& kc, const std::function<void()>& fn, bool autoRepeat) override final;
    int specializedRemove(const KeyCombination& kc) override final;
    int specializedRemoveAll() override final;
    int specializedReplace(const KeyCombination& oldKc, const KeyCombination& newKc) override final;
    int specializedSetAutoRepeat(const KeyCombination& kc, bool autoRepeat) override final;

    void eachCycleDo() override final;

    virtual void work() = 0;
    virtual int workOfEnd() = 0;
    virtual int workOfAdd(const KeyCombination& kc, bool autoRepeat) = 0;
    virtual int workOfRemove(const KeyCombination& kc) = 0;
    virtual int workOfRemoveAll() = 0;
    virtual int workOfReplace(const KeyCombination& oldKc, const KeyCombination& newKc) = 0;
    virtual int workOfSetAutoRepeat(const KeyCombination& kc, bool autoRepeat) = 0;

private:
    struct Task
    {
        enum Operator
        {
            NONE,
            END,
            ADD,
            REMOVE,
            REMOVE_ALL,
            REPLACE,
            SET_AUTO_REPEAT
        };

        Operator op;
        union
        {
            struct
            {
                KeyCombination kc;
                bool autoRepeat;
            } add;
            struct
            {
                KeyCombination kc;
            } remove;
            struct
            {
                KeyCombination oldKc;
                KeyCombination newKc;
            } replace;
            struct
            {
                KeyCombination kc;
                bool autoRepeat;
            } setAutoRepeat;
        };

        Task() : op(NONE), replace() {}
    };

    void setTask(const Task& task);
    Task getTask();
    int waitTaskFinished();

    std::mutex mtx;
    Task task;
    std::condition_variable cvTaskFinished;
    std::atomic<bool> taskFinished;
    std::atomic<int> taskReturnCode;
};

} // namespace gbhk

#endif // !GLOBAL_HOTKEY_DISABLE_REGISTER

#endif // !GLOBAL_HOTKEY_REGISTER_GHM_PRIVATE_HPP
