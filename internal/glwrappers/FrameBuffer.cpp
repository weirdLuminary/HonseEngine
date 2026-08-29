#include "FrameBuffer.h"

FrameBuffer::FrameBuffer(bool multisample) : m_Multisample(multisample) {
    glGenFramebuffers(1, &m_RendererID);
}

FrameBuffer::~FrameBuffer() {
    if(m_RendererID != 0)
        glDeleteFramebuffers(1, &m_RendererID);
}

void FrameBuffer::AttachTexture(int w, int h) {

    glGenTextures(1, &m_Texture);

    if(m_Multisample) {
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_Texture);
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB, w, h, GL_TRUE);
    }
    else {
        glBindTexture(GL_TEXTURE_2D, m_Texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    glBindTexture(GL_TEXTURE_2D, 0);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_Multisample ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D, m_Texture, 0); 

}

void FrameBuffer::Blit(int w, int h) {
    
    glBlitFramebuffer(0, 0, w, h, 0, 0, w, h, GL_COLOR_BUFFER_BIT, GL_NEAREST);
}

const GLuint FrameBuffer::GetTexture() const {
    return m_Texture;
}

bool FrameBuffer::CheckComplete() {
    return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
}

void FrameBuffer::ResizeTexture(int w, int h) const
{
    if (m_Multisample)
    {
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_Texture);

        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB, w, h, GL_TRUE);

        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
    }
    else
    {
        glBindTexture(GL_TEXTURE_2D, m_Texture);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

        glBindTexture(GL_TEXTURE_2D, 0);
    }
}
void FrameBuffer::Bind(bool read, bool write) const {
    if(!read && !write) return;

    GLenum target;

    if(read && write) 
        target = GL_FRAMEBUFFER;
    else if(read && !write) 
        target = GL_READ_FRAMEBUFFER;
    else if(!read && write) 
        target = GL_DRAW_FRAMEBUFFER;

    glBindFramebuffer(target, m_RendererID);
}

void FrameBuffer::Unbind(bool read, bool write) {
    if(!read && !write) return;

    GLenum target;

    if(read && write) 
        target = GL_FRAMEBUFFER;
    else if(read && !write) 
        target = GL_READ_FRAMEBUFFER;
    else if(!read && write) 
        target = GL_DRAW_FRAMEBUFFER;

    glBindFramebuffer(target, 0);
}