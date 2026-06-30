#pragma once
#include <glad/glad.h>

class IndexBuffer {

private:

    GLuint m_RendererID;

public:

    IndexBuffer(const GLuint data[], unsigned int count);
    ~IndexBuffer();

    void Bind() const;
    void Unbind() const;
    

};