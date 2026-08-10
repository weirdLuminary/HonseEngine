#pragma once
#include <glad/glad.h>

class FrameBuffer {

private:

    GLuint m_RendererID = 0;
    GLuint m_Texture = 0;

public:

    FrameBuffer();
    ~FrameBuffer();

    void Bind() const;
    static void Unbind();
    static bool CheckComplete();

    void AttachTexture(int width, int height);
    void ResizeTexture(int width, int height) const;
    const GLuint GetTexture() const;

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
            if (m_Texture != 0)
                glDeleteTextures(1, &m_Texture);

            if (m_RendererID != 0)
                glDeleteFramebuffers(1, &m_RendererID);

            m_RendererID = other.m_RendererID;
            m_Texture = other.m_Texture;
            other.m_RendererID = 0;
            other.m_Texture = 0;
        }
        return *this;
    }

};