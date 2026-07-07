#include <honse/modules/File.h>
#include <cstdio>


hs::File::File(const std::string& path) {

    m_File = fopen(path.c_str(), "r");

    if(m_File == NULL) {
        printf("File \'%s\' failed to load!", path);
        return;
    }

}

void hs::File::Close() {
    if(m_File) fclose(m_File);
}

hs::File::~File() {

    Close();

}

const std::string hs::File::Read() {

    if(m_File == NULL) return "";

    std::string text;
    char buffer[1024];

    while (fgets(buffer, sizeof(buffer), m_File))
    {
        text += buffer;
    }

    return text;

}