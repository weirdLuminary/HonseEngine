#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(const GLuint data[], unsigned int count) {

    glGenBuffers(1, &m_RendererID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), data, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

IndexBuffer::~IndexBuffer() {
    if (m_RendererID != 0)
        glDeleteBuffers(1, &m_RendererID);
}

void IndexBuffer::SetData(const void* data, unsigned int size) const {

    Bind();
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, size, data);
    Unbind();
    
}

void IndexBuffer::Bind() const {

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);

}

void IndexBuffer::Unbind() {

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}