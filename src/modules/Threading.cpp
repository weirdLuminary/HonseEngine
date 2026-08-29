#include <honse/modules/Threading.h>
#include <iostream>

std::vector<std::thread> honse::Threading::m_Threads;
std::queue<std::function<void()>> honse::Threading::m_ThreadPool;

std::atomic<bool> honse::Threading::m_Running = true;
std::mutex honse::Threading::m_Mutex;
std::condition_variable honse::Threading::m_Condition;
unsigned int honse::Threading::m_ActiveThreads;

void honse::Threading::Init() {
    const uint8_t workers = 8;

    m_Threads.reserve(workers);

    for (int i = 0; i < workers; i++) {
        m_Threads.emplace_back(std::thread([]() { RunPool(); }));
    }
    printf("Created %i worker threads\n", workers);
}

void honse::Threading::Wait() {

    std::unique_lock lock(m_Mutex);
    m_Condition.wait(
        lock, [] { return m_ThreadPool.empty() && m_ActiveThreads <= 0; });
}

void honse::Threading::Shutdown() {

    m_Running = false;

    m_Condition.notify_all();

    for (auto& thread : m_Threads)
        thread.join();
}

void honse::Threading::Enqueue(std::function<void()> func) {

    {
        std::lock_guard lock(m_Mutex);
        m_ThreadPool.push(std::move(func));
    }

    m_Condition.notify_one();
}

void honse::Threading::RunPool() {

    while (true) {

        std::function<void()> func;

        {

            std::unique_lock lock(m_Mutex);

            m_Condition.wait(
                lock, [] { return !m_ThreadPool.empty() || !m_Running; });

            if (!m_Running)
                return;

            func = std::move(m_ThreadPool.front());
            m_ThreadPool.pop();

            m_ActiveThreads++;
        }

        func();

        {
            std::lock_guard lock(m_Mutex);

            --m_ActiveThreads;

            if (m_ThreadPool.empty() && m_ActiveThreads == 0)
                m_Condition.notify_all();
        }
    }
}