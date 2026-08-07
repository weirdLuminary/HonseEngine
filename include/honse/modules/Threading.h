#pragma once
#include <thread>
#include <array>
#include <queue>
#include <atomic>
#include <functional>
#include <mutex>
#include <condition_variable>

namespace honse {

    class Threading {

    public:

        Threading() = delete;

        static void Init();
        static void Shutdown();

        static void Enqueue(std::function<void()> func);
        static void Wait();

    private:

        static void RunPool();

        static std::vector<std::thread> m_Threads;
        static std::queue<std::function<void()>> m_ThreadPool;

        static std::atomic<bool> m_Running;
        static unsigned int m_ActiveThreads;
        static std::mutex m_Mutex;
        static std::condition_variable m_Condition;

    };

}