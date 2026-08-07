#pragma once
#include <glad/glad.h>

class IndexBuffer {

private:

    GLuint m_RendererID;

public:

    void Init(const GLuint data[], unsigned int count);

    IndexBuffer(const GLuint data[], unsigned int count);
    IndexBuffer() = default;
    ~IndexBuffer();

    void Bind() const;
    static void Unbind();
    void SetData(const void* data, unsigned int size) const;

    IndexBuffer(IndexBuffer&& other) noexcept
    : m_RendererID(other.m_RendererID)
    {
        other.m_RendererID = 0;
    }

    IndexBuffer& operator=(IndexBuffer&& other) noexcept
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