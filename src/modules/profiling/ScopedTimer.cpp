#include <honse/modules/profiling/ScopedTimer.h>

honse::ScopedTimer::~ScopedTimer()
{
    #ifdef DEBUG
    auto end = std::chrono::high_resolution_clock::now();

    auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - m_Start);

    printf("%s: %lld ns\n", m_Name, ns.count());
    #endif
}