#include <honse/graphics/Sprite.h>

hs::Sprite::Sprite(Texture* texture) {
    material = hs::FromTexture(texture);
}

glm::vec2 hs::Sprite::GetSize() const {
    return glm::vec2(material->texture->width, material->texture->height);
}