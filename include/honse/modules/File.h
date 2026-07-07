#pragma once
#include <stdio.h>
#include <string>

namespace hs {

    class File {

    public:

        File(const std::string& path);
        ~File();

        const std::string Read();
        void Close();

    private:

        FILE* m_File;

    };

};