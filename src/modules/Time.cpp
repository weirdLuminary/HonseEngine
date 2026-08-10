#include <honse/modules/Time.h>
#include <honse/modules/profiling/Profiling.h>


float honse::Time::m_DeltaTime;
std::chrono::time_point<std::chrono::high_resolution_clock> honse::Time::m_LastFrameStart;
uint64_t honse::Time::m_Time;

void honse::Time::StartFrame() {
    m_LastFrameStart = std::chrono::high_resolution_clock::now();
}

void honse::Time::EndFrame() {
    auto now =  std::chrono::high_resolution_clock::now();

    m_DeltaTime = std::chrono::duration<float>(now - m_LastFrameStart).count();

    honse::Profiling::Set("ΔT (seconds)", m_DeltaTime);
}


