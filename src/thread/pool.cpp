////////////////////////
//
//  Created: Wed Apr 08 2026
//  File: pool.cpp
//
////////////////////////

#include "thread/pool.hpp"

namespace ecs
{
    ThreadPool::ThreadPool(u64 threads)
        : _threads()
        , _jobs()
        , _mutex()
        , _cv()
        , _stop()
    {
        for (u64 i = 0; i < threads; ++i)
            _threads.emplace_back(&ThreadPool::workerLoop, this);
    }

    ThreadPool::~ThreadPool()
    {
        {
            std::lock_guard<std::mutex> lock(_mutex);
            _stop = true;
        }

        _cv.notify_all();

        for (std::thread &thread : _threads)
            thread.join();
    }

    u64 ThreadPool::getNumThreads() const
    {
        return _threads.size();
    }

    void ThreadPool::enqueue(Job job)
    {
        {
            std::lock_guard<std::mutex> lock(_mutex);
            _jobs.push(std::move(job));
        }

        _cv.notify_one();
    }

    void ThreadPool::workerLoop()
    {
        while (true) {
            Job job = nullptr;

            {
                std::unique_lock<std::mutex> lock(_mutex);

                _cv.wait(lock, [this] {
                    return _stop || !_jobs.empty();
                });

                if (_stop && _jobs.empty())
                    return;

                job = std::move(_jobs.front());
                _jobs.pop();
            }

            if (job)
                job();
        }
    }
}
