#include <honse/graphics/Material.h>

namespace hs
{
    
    Material* FromTexture(Texture* tex) { 
        return new Material { tex, new Shader() };
    }

} 
