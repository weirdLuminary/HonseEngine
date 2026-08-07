#include <honse/graphics/Texture.h>
#include <honse/graphics/Shader.h>

namespace honse {

    class Material
    {

    public:
        Texture* texture;
        Shader* shader;

        Material(Texture* texture);
        Material(Texture* texture, Shader* shader);

    };

}