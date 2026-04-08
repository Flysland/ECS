////////////////////////
//
//  Created: Wed Apr 08 2026
//  File: jobs.cpp
//
////////////////////////

#include "thread/jobs.hpp"

namespace ecs
{
    void Jobs::run(ThreadPool &pool, Job job)
    {
        _remaining.fetch_add(1, std::memory_order_release);

        pool.enqueue([this, job] {
            job();

            _remaining.fetch_sub(1, std::memory_order_release);

            std::lock_guard<std::mutex> lock(_mutex);
            _cv.notify_all();
        });
    }

    void Jobs::wait()
    {
        std::unique_lock<std::mutex> lock(_mutex);

        _cv.wait(lock, [this] {
            return _remaining.load(std::memory_order_acquire) == 0;
        });
    }

    u64 Jobs::remaining() const
    {
        return _remaining.load(std::memory_order_acquire);
    }
}
