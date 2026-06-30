#include <honse/graphics/Texture.h>
#include <stb_image.h>

Texture::Texture(const std::string& path) {
    
    data = stbi_load(path.c_str(), &width, &height, &channels, 0);

    if(!data) {
        printf("Failed to load texture from \'%s\'!", path.c_str());
        stbi_image_free(data);
        return;
    }

    glGenTextures(1, &m_RendererID);

    glBindTexture(GL_TEXTURE_2D, m_RendererID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, channels == 4 ? GL_RGBA : GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    //glGenerateMipmap(GL_TEXTURE_2D);
}

Texture::~Texture() {

    stbi_image_free(data);

}

void Texture::Bind(GLenum slot) const {

    glActiveTexture(slot);
    glBindTexture(GL_TEXTURE_2D, m_RendererID);

}