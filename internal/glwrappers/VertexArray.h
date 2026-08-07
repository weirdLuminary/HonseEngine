#pragma once
#include "VertexBuffer.h"
#include <vector>

enum class VertexRate
{
    Vertex,
    Instance
};

struct VertexAttribute {

    GLuint location;
    GLint count;
    GLenum type;
    GLboolean normalized = GL_FALSE;

    static GLuint GetSize(GLenum type) 
    {
        switch (type)
        {
            case GL_FLOAT:
                return 4;
            
            case GL_INT:
            case GL_UNSIGNED_INT:
                return 4;

            case GL_BOOL:
            case GL_UNSIGNED_BYTE:
            case GL_BYTE:
                return 1;
        }
        return 0;
    }

};

class VertexArray {

private:

    GLuint m_RendererID = 0;

public:

    void Init();

    VertexArray();
    
    void Bind() const;
    static void Unbind();

    void AddBuffer(const VertexBuffer& vb, const std::vector<VertexAttribute>& layout, VertexRate rate = VertexRate::Vertex);

    VertexArray(const VertexArray&) = delete;
    VertexArray& operator=(const VertexArray&) = delete;

    VertexArray(VertexArray&& other) noexcept
    : m_RendererID(other.m_RendererID)
    {
        other.m_RendererID = 0;
    }

    VertexArray& operator=(VertexArray&& other) noexcept
    {
        if (this != &other)
        {
            if (m_RendererID != 0)
                glDeleteVertexArrays(1, &m_RendererID);  

            m_RendererID = other.m_RendererID;
            other.m_RendererID = 0;
        }
        return *this;
    }


};