#include "Texture.h"
#include "Shader.h"

namespace hs {

    struct Material
    {
        Texture* texture;
        Shader* shader;
    };

    Material* FromTexture(Texture* texture);

}