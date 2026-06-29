#include "VertexBuffer.h"
#include <stdio.h>

VertexBuffer::VertexBuffer(const void* data, unsigned int size) 
{
    glGenBuffers(1, &m_RendererID);
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    printf("Created buffer %d!\n", m_RendererID);
}

VertexBuffer::~VertexBuffer()
{
    if(m_RendererID == 0) return;
    printf("Buffer %d removed!\n", m_RendererID);
    glDeleteBuffers(1, &m_RendererID);
}

void VertexBuffer::Bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
}

void VertexBuffer::Unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}