#pragma once
#include <stdio.h>
#include <string>

namespace hs {

    class File {

    public:

        File(std::string& path, bool binary = false);
        ~File();

        std::string Read();
        void Write(std::string value, bool override = true);

        void WriteBinary();

        void Close();

    private:

        void Open(const char* args);

        bool m_Binary;
        std::string m_Path;
        FILE* m_File;

    };

}