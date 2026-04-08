////////////////////////
//
//  Created: Wed Apr 08 2026
//  File: jobs.hpp
//
////////////////////////

#pragma once

#include "thread/pool.hpp"

namespace ecs
{
    class Jobs
    {
        using Job = std::function<void()>;

        public:
            void run(ThreadPool &pool, Job job);
            void wait();
            u64 remaining() const;

        private:
            std::atomic<u64> _remaining;
            std::mutex _mutex;
            std::condition_variable _cv;
    };
}
