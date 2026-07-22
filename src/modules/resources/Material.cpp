#include <honse/modules/resources/Material.h>
#include <honse/modules/resources/ResourceManager.h>

hs::Material::Material(Texture* tex) : texture(tex) { 
    shader = hs::ResourceManager::Load<hs::Shader>("basic").get();

}


