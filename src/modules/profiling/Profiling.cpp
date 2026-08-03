#include <honse/modules/profiling/Profiling.h>
#include <stdio.h>
#include <iostream>

std::unordered_map<const char*, honse::ProfileValue> honse::Profiling::m_Info;

void honse::Profiling::Set(const char* name, ProfileValue value) {
    m_Info.insert_or_assign(name, value);
}

void honse::Profiling::FlushData() {
    for (const auto& data : m_Info) {
        printf("%s: ", data.first);

        std::visit([](const auto& value)
        {
            std::cout << value;
        }, data.second);

        printf("\n");
    }

    m_Info.clear();
}