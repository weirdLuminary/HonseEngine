#pragma once
#include <chrono>
#include <stdio.h>

namespace honse {

    class ScopedTimer
    {
    public:
        ScopedTimer(const char* name)
            : m_Name(name)
        {
            #ifdef DEBUG
            m_Start = std::chrono::high_resolution_clock::now();
            #endif
        }

        ~ScopedTimer();

    private:

        const char* m_Name;
        std::chrono::high_resolution_clock::time_point m_Start;
    };

}