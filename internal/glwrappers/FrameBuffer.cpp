#include "FrameBuffer.h"

FrameBuffer::FrameBuffer() {
    glGenFramebuffers(1, &m_RendererID);
}

FrameBuffer::~FrameBuffer() {
    if(m_RendererID != 0)
        glDeleteFramebuffers(1, &m_RendererID);
}

void FrameBuffer::Bind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
}

void FrameBuffer::Unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}