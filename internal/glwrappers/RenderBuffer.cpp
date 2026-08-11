#include "RenderBuffer.h"

RenderBuffer::RenderBuffer(int w, int h) {
    glGenRenderbuffers(1, &m_RendererID);
    glBindRenderbuffer(GL_RENDERBUFFER, m_RendererID);

    glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, w, h);

    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

RenderBuffer::~RenderBuffer() {
    if(m_RendererID != 0)
        glDeleteRenderbuffers(1, &m_RendererID);
}

void RenderBuffer::AttachFramebuffer() const {
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RendererID);
}

void RenderBuffer::Resize(int w, int h) const {
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, w, h);
}

void RenderBuffer::Bind() const {
    glBindRenderbuffer(GL_RENDERBUFFER, m_RendererID);
}

void RenderBuffer::Unbind() {
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}