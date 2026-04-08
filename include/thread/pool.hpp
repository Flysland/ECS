////////////////////////
//
//  Created: Wed Apr 08 2026
//  File: pool.hpp
//
////////////////////////

#pragma once

#include <condition_variable>
#include <functional>
#include <queue>
#include <thread>

#include "types.hpp"

namespace ecs
{
    class ThreadPool
    {
        using Job = std::function<void()>;

        friend class Jobs;

        public:
            explicit ThreadPool(u64 threads = std::thread::hardware_concurrency());
            ~ThreadPool();

            u64 getNumThreads() const;

        private:
            std::vector<std::thread> _threads;
            std::queue<Job> _jobs;
            std::mutex _mutex;
            std::condition_variable _cv;
            bool _stop;

            void enqueue(Job job);
            void workerLoop();
    };
}
