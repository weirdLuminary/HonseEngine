#include "FrameBuffer.h"

FrameBuffer::FrameBuffer() {
    glGenFramebuffers(1, &m_RendererID);
}

FrameBuffer::~FrameBuffer() {
    if(m_RendererID != 0)
        glDeleteFramebuffers(1, &m_RendererID);
}

void FrameBuffer::AttachTexture(int w, int h) {

    glGenTextures(1, &m_Texture);
    glBindTexture(GL_TEXTURE_2D, m_Texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_Texture, 0); 

}

const GLuint FrameBuffer::GetTexture() const {
    return m_Texture;
}

bool FrameBuffer::CheckComplete() {
    return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
}

void FrameBuffer::ResizeTexture(int w, int h) const {
    glBindTexture(GL_TEXTURE_2D, m_Texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void FrameBuffer::Bind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
}

void FrameBuffer::Unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}