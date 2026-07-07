#include <honse/graphics/Renderer.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <VertexArray.h>
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "SpriteVertex.h"
#include <honse/graphics/Texture.h>

#include <iostream>

using namespace hs;

std::unique_ptr<hs::Renderer::Impl> hs::Renderer::impl;

void GLAPIENTRY GLDebugCallback(
        GLenum source,
        GLenum type,
        GLuint id,
        GLenum severity,
        GLsizei length,
        const GLchar* message,
        const void* userParam)
    {
        printf("[GL DEBUG] %s\n", message);
    }

struct Renderer::Impl {

    VertexArray quadVA;
    VertexBuffer quadVB;
    IndexBuffer quadEB;

    glm::mat4 projection;
    Shader defaultShader;

    std::vector<SpriteVertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<GLuint> textureSlots;

    unsigned int spriteCount = 0;
    const size_t MAX_SPRITES = 1000;
    GLint MAX_TEXTURES = 16;

    

    void initRenderData() {

        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

        glDebugMessageCallback(GLDebugCallback, nullptr);

        //glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &MAX_TEXTURES);

        vertices.reserve(MAX_SPRITES * 4);
        indices.resize(MAX_SPRITES * 6);

        for (unsigned int i = 0; i < MAX_SPRITES; ++i)
        {
            unsigned int vertex = i * 4;
            unsigned int index  = i * 6;

            indices[index + 0] = vertex + 0;
            indices[index + 1] = vertex + 1;
            indices[index + 2] = vertex + 2;

            indices[index + 3] = vertex + 2;
            indices[index + 4] = vertex + 1;
            indices[index + 5] = vertex + 3;
        }

        projection = glm::ortho(-800.0f, 800.0f, -800.0f, 800.0f);
        defaultShader = Shader();

        int samplers[16];

        for (int i = 0; i < 16; i++)
            samplers[i] = i;

        defaultShader.Bind();
        defaultShader.FindUniform("u_Textures").Set(samplers, 16);

        quadVA.Init();
        quadVA.Bind();

        quadVB.Init(nullptr, MAX_SPRITES * sizeof(SpriteVertex) * 4);
        quadEB.Init(indices.data(), indices.size() * sizeof(unsigned int));

        quadVB.Bind();
        quadEB.Bind();

        quadVA.Bind();
        
        quadVA.AddBuffer(quadVB, {
            { 0, 4, GL_FLOAT }, // Position
            { 1, 2, GL_FLOAT }, // UV
            { 2, 4, GL_FLOAT },  // Color
            { 3, 1, GL_INT   } // Texture ID
        });

    }

    int GetTextureSlot(GLuint handle)
    {
        for (GLuint i = 0; i < textureSlots.size(); ++i)
        {
            if (textureSlots[i] == handle)
                return i;
        }

        if (textureSlots.size() >= MAX_TEXTURES)
        {
            // Flush should happen before adding a new texture
            return -1;
        }

        textureSlots.push_back(handle);
        return textureSlots.size() - 1;
    }


};

/////////////////////////////////////////////

void Renderer::Init() {

    impl = std::make_unique<Impl>();
    impl->initRenderData();
}

void Renderer::Shutdown() {

    impl.reset();
}



void Renderer::Submit(const hs::Sprite& sprite) {

    if (impl->spriteCount >= impl->MAX_SPRITES ||
    impl->textureSlots.size() >= impl->MAX_TEXTURES)
    {
        Flush();
    }

    ///// CALCULATE TRANSFORM

    auto& position = sprite.position;
    auto& pivot = sprite.pivot;
    auto size = sprite.GetSize();

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position - pivot * size, 0.0f)); 

    model = glm::translate(model, glm::vec3(pivot.x * size.x, pivot.y * size.y, 0.0f)); 
    model = glm::rotate(model, glm::radians(sprite.rotation), glm::vec3(0.0f, 0.0f, 1.0f)); 
    model = glm::translate(model, glm::vec3(-pivot.x * size.x, -pivot.y * size.y, 0.0f)); // Bring vertices back so that the pivot behaves as it should

    model = glm::scale(model, glm::vec3(size, 1.0f)); 

    ///// GENERATE VERTICES

    int slot = impl->GetTextureSlot(sprite.material->texture->GetHandle());

    if (slot == -1)
    {
        Flush();
        impl->textureSlots.clear();
        slot = impl->GetTextureSlot(sprite.material->texture->GetHandle());
    }

    SpriteVertex vertices[] = {  // Position               // Texture coords    // Tex slot and color
        SpriteVertex { glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f), sprite.color, slot  },
        SpriteVertex { glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f), sprite.color, slot  },
        SpriteVertex { glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f), sprite.color, slot  },
        SpriteVertex { glm::vec4(1.0f, 1.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f), sprite.color, slot  }
    };

    for(SpriteVertex& vertex : vertices) {
        vertex.position = impl->projection * model * vertex.position; // Apply transform + projection
        impl->vertices.push_back(vertex);
    }

    impl->spriteCount++;
    
};

void Renderer::Begin() {
    impl->textureSlots.clear();
    impl->spriteCount = 0;
    impl->vertices.clear();
}

void Renderer::End() {
    impl->quadVA.Unbind();
    impl->defaultShader.Unbind();
}

void Renderer::Flush() {

    impl->quadVA.Bind();
    impl->quadVB.Bind();
    impl->quadEB.Bind();
    impl->defaultShader.Bind();

    impl->quadVB.SetData(impl->vertices.data(), impl->vertices.size() * sizeof(SpriteVertex));
    // Note: EBO data is already set in init dumass

    glClear(GL_COLOR_BUFFER_BIT);

    for (GLuint i = 0; i < impl->textureSlots.size(); ++i)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, impl->textureSlots[i]);
    }

    glDrawElements(GL_TRIANGLES, 6 * impl->spriteCount, GL_UNSIGNED_INT, 0);

    

    impl->spriteCount = 0;
    impl->vertices.clear();

};

