#include <honse/modules/resources/Material.h>
#include <honse/modules/resources/ResourceManager.h>

honse::Material::Material(Texture* tex) : texture(tex) { 
    shader = honse::ResourceManager::Load<honse::Shader>("basic").get();

}


