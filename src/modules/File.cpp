#include <honse/modules/File.h>
#include <cstdio>


honse::File::File(std::string& path, bool binary) : m_Path(path), m_Binary(binary) {}

void honse::File::Close() {
    if (m_File != nullptr)
    {
        fclose(m_File);
        m_File = nullptr;
    }
}

void honse::File::Open(const char* args) {

    if(m_File != NULL) {
        Close();
    }

    m_File = fopen(m_Path.c_str(), args);

    if(m_File == NULL) {
        printf("File failed to open!");
    }
}


honse::File::~File() {

    Close();

}

std::string honse::File::Read() {

    Open("r");

    std::string text;
    char buffer[1024];

    while (fgets(buffer, sizeof(buffer), m_File)) {
        text += buffer;
    }

    Close();

    return text;

}

void honse::File::Write(std::string value, bool override) {

    Open(override ? "w" : "a");

    fputs(value.c_str(), m_File);

    Close();

}