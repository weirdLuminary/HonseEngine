#include <honse/modules/Time.h>
#include <honse/modules/profiling/Profiling.h>

float honse::Time::m_DeltaTime;
std::chrono::time_point<std::chrono::high_resolution_clock>
    honse::Time::m_LastFrameStart;
std::chrono::time_point<std::chrono::high_resolution_clock>
    honse::Time::m_Start;
float honse::Time::m_Time;

void honse::Time::Init() {
    m_Start = std::chrono::high_resolution_clock::now();
}

void honse::Time::StartFrame() {
    m_LastFrameStart = std::chrono::high_resolution_clock::now();
}

void honse::Time::EndFrame() {
    auto now = std::chrono::high_resolution_clock::now();

    m_DeltaTime = std::chrono::duration<float>(now - m_LastFrameStart).count();
    m_Time = std::chrono::duration<float>(now - m_Start).count();

    honse::Profiling::Set("ΔT (seconds)", m_DeltaTime);
}
