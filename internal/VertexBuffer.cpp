#include "VertexBuffer.h"
#include <stdio.h>

VertexBuffer::VertexBuffer(const void* data, unsigned int size) 
{
    glGenBuffers(1, &m_RendererID);
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    printf("Created buffer %d!\n", m_RendererID);
}

void VertexBuffer::Init(const void* data, unsigned int size) {
    glGenBuffers(1, &m_RendererID);
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    printf("Created buffer %d!\n", m_RendererID);
}

VertexBuffer::~VertexBuffer()
{
    if (m_RendererID != 0)
        glDeleteBuffers(1, &m_RendererID);
}

void VertexBuffer::SetData(const void* data, unsigned int size) const {

    Bind();
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);

}

void VertexBuffer::Bind() const {

    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
}

void VertexBuffer::Unbind() {
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}