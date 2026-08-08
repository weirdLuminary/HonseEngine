#pragma once
#include <chrono>

namespace honse
{
   
    class Time {

    public:

        Time() = delete;

        static const float GetDeltaTime() { return m_DeltaTime; }
        static const uint64_t GetTime() { return m_Time; }

    private:

        static void StartFrame();
        static void EndFrame();

        static float m_DeltaTime;
        static std::chrono::time_point<std::chrono::high_resolution_clock> m_LastFrameStart;

        static uint64_t m_Time;

        friend class Engine;

    };

} 
