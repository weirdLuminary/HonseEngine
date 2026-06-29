#pragma once
#include <glad/glad.h>

class VertexBuffer {

private:

    GLuint m_RendererID;

public:

    VertexBuffer() : m_RendererID(0) {}
    VertexBuffer(const void* data, unsigned int size);
    ~VertexBuffer();

    void Bind() const;
    void Unbind() const;

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
            glDeleteBuffers(1, &m_RendererID);

            m_RendererID = other.m_RendererID;
            other.m_RendererID = 0;
        }
        return *this;
    }

};