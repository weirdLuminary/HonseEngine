#pragma once
#include <glad/glad.h>

class VertexBuffer {

private:

    GLuint m_RendererID = 0;

public:

    VertexBuffer() = default;
    VertexBuffer(const void* data, unsigned int size, GLenum type = GL_STATIC_DRAW);
    ~VertexBuffer();

    void Bind() const;
    static void Unbind();
    void SetData(const void* data, unsigned int size) const;

    // Remove copying
    VertexBuffer(const VertexBuffer&) = delete;
    VertexBuffer& operator=(const VertexBuffer&) = delete;

    VertexBuffer(VertexBuffer&& other) noexcept
    : m_RendererID(other.m_RendererID)
    {
        other.m_RendererID = 0;
    }

    VertexBuffer& operator=(VertexBuffer&& other) noexcept
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