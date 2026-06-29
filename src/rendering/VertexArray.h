#pragma once
#include "VertexBuffer.h"
#include <vector>

struct VertexAttribute {

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
                return 1;
        }
        return 0;
    }

};

class VertexArray {

private:

    GLuint m_RendererID = 0;

public:

    VertexArray();
    
    void Bind() const;
    void Unbind() const;

   void AddBuffer(const VertexBuffer& vb, const std::vector<VertexAttribute>& layout);


};