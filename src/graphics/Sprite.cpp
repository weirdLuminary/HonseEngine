#include <honse/graphics/Sprite.h>

hs::Sprite::Sprite(Material* material) {
    this->material = material;
}

glm::vec2 hs::Sprite::GetSize() const {
    return glm::vec2(material->texture->width, material->texture->height);
}