#pragma once
#include <glad/glad.h>

class FrameBuffer {

private:

    GLuint m_RendererID;


public:

    FrameBuffer();
    ~FrameBuffer();

    void Bind() const;
    static void Unbind();

    // Remove copying
    
    FrameBuffer(const FrameBuffer&) = delete;
    FrameBuffer& operator=(const FrameBuffer&) = delete;

    FrameBuffer(FrameBuffer&& other) noexcept
    : m_RendererID(other.m_RendererID)
    {
        other.m_RendererID = 0;
    }

    FrameBuffer& operator=(FrameBuffer&& other) noexcept
    {
        if (this != &other)
        {
            if (m_RendererID != 0)
                glDeleteBuffers(1, &m_RendererID);  

            m_RendererID = other.m_RendererID;
            other.m_RendererID = 0;
        }
        return *this;
    }

};