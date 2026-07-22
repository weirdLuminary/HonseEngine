#include <honse/graphics/Texture.h>
#include <honse/graphics/Shader.h>

namespace hs {

    class Material
    {

    public:
        Texture* texture;
        Shader* shader;

        Material(Texture* texture);
        Material(Texture* texture, Shader* shader);

    };

}