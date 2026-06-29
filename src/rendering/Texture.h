#pragma once
#include <glad/glad.h>
#include <string>

class Texture {

private:

    GLuint m_RendererID = 0;

public:

    int width, height, channels;
    unsigned char *data;

    Texture(const std::string& path);
    ~Texture();

    void Bind(GLenum slot) const;
    void Unbind() const;


};