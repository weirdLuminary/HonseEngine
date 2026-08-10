#pragma once
#include <glad/glad.h>

class RenderBuffer {

private:

    GLuint m_RendererID;


public:

    RenderBuffer() = default;
    RenderBuffer(int width, int height);
    ~RenderBuffer();

    void AttachFramebuffer() const;
    void Resize(int width, int height) const;

    void Bind() const;
    static void Unbind();

    // Remove copying
    
    RenderBuffer(const RenderBuffer&) = delete;
    RenderBuffer& operator=(const RenderBuffer&) = delete;

    RenderBuffer(RenderBuffer&& other) noexcept
    : m_RendererID(other.m_RendererID)
    {
        other.m_RendererID = 0;
    }

    RenderBuffer& operator=(RenderBuffer&& other) noexcept
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