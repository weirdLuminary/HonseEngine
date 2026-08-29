#include <cassert>
#include <glad/glad.h>
#include <honse/graphics/Texture.h>
#include <stb_image.h>

using namespace honse;

Texture::Texture(Texture&&) noexcept = default;
Texture& Texture::operator=(Texture&&) noexcept = default;

struct Texture::Impl {

    unsigned char* data = nullptr;

    GLuint rendererID = 0;

    friend class Renderer;
};

unsigned int Texture::GetHandle() { return impl->rendererID; }

void Texture::Bind(unsigned int slot) const {

    glActiveTexture(slot);
    glBindTexture(GL_TEXTURE_2D, impl->rendererID);
}

Texture::Texture() : impl(std::make_unique<Impl>()) {}

Texture::Texture(const std::string& path) : impl(std::make_unique<Impl>()) {
    glm::vec<2, int> sizeInt;
    impl->data = stbi_load(path.c_str(), &sizeInt.x, &sizeInt.y, &channels, 0);

    size.x = sizeInt.x;
    size.y = sizeInt.y;

    if (!impl->data) {
        printf("Failed to load texture from \'%s\'!", path.c_str());
        printf("%s\n", stbi_failure_reason());
        stbi_image_free(impl->data);
        return;
    }

    assert(channels > 0 && "Suspicious channel value");
    assert(size.x < 10000 && size.y < 10000 && "Suspicious size values");

    glGenTextures(1, &impl->rendererID);

    glBindTexture(GL_TEXTURE_2D, impl->rendererID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    GLenum format = channels == 4 ? GL_RGBA : GL_RGB;

    glTexImage2D(GL_TEXTURE_2D, 0, format, size.x, size.y, 0, format,
                 GL_UNSIGNED_BYTE, impl->data);
    // glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(impl->data);
}

Texture::~Texture() {
    if (impl && impl->rendererID != 0)
        glDeleteTextures(1, &impl->rendererID);
}