#pragma once

#include <condition_variable>
#include <mutex>
#include <queue>

template <typename T>
class Queue
{
public:
    void push(const T& item)
    {
        std::lock_guard<std::mutex> lock(mtx);
        queue.emplace(item);
        if (queue.size() == 1)
            cvHas.notify_all();
    }

    T take()
    {
        std::unique_lock<std::mutex> lock(mtx);
        cvHas.wait(lock, [this]() { return !queue.empty(); });
        T item = queue.front();
        queue.pop();
        return item;
    }

    void clear()
    {
        std::lock_guard<std::mutex> lock(mtx);
        while (!queue.empty())
            queue.pop();
    }

private:
    std::mutex mtx;
    std::condition_variable cvHas;
    std::queue<T> queue;
};
