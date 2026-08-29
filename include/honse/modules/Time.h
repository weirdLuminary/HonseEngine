#pragma once
#include <chrono>

namespace honse
{
   
    class Time {

    public:

        Time() = delete;

        static void Init();

        static const float GetDeltaTime() { return m_DeltaTime; }
        static const float GetTime() { return m_Time; }

    private:

        static void StartFrame();
        static void EndFrame();

        static float m_DeltaTime;
        static std::chrono::time_point<std::chrono::high_resolution_clock> m_LastFrameStart;
        static std::chrono::time_point<std::chrono::high_resolution_clock> m_Start;

        static float m_Time;

        friend class Engine;

    };

} 
