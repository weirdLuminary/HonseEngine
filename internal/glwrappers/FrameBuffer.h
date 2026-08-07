#pragma once
#include <glad/glad.h>

class FrameBuffer {

private:

    GLuint m_RendererID;


public:

    void Bind() const;
    static void Unbind();

    FrameBuffer();
    ~FrameBuffer();

};