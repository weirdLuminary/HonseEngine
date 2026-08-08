#include <honse/graphics/Renderer.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "glwrappers/VertexArray.h"
#include "glwrappers/IndexBuffer.h"
#include "glwrappers/VertexBuffer.h"
#include "glwrappers/SpriteInstance.h"
#include "glwrappers/QuadVertex.h"
#include "glwrappers/Query.h"

#include <honse/graphics/Texture.h>
#include <honse/graphics/Camera.h>
#include <honse/graphics/Shader.h>
#include <honse/modules/profiling/Profiling.h>
#include <honse/modules/resources/ResourceManager.h>

#include <iostream>
#include <chrono>

using namespace honse;


float honse::deltaTime = 0.0f;
std::unique_ptr<honse::Renderer::Impl> honse::Renderer::impl;

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
    VertexBuffer instanceVB;
    IndexBuffer quadEB;

    glm::mat4 projection;
    glm::mat4 view;

    Query frameQuery;
    Query primitiveQuery;
    unsigned int drawCalls;

    std::vector<SpriteInstance> instances; // Per-sprite settings

    std::unordered_map<GLuint, int> textureSlotMap;
    std::vector<GLuint> textureSlots;

    std::shared_ptr<Shader> shader;

    unsigned int spriteCount = 0;
    const size_t MAX_SPRITES = 10000;
    GLint MAX_TEXTURES = 16;

    void initRenderData() {

        printf("%s\n", glGetString(GL_VERSION));
        // glEnable(GL_DEBUG_OUTPUT);
        // glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 

        glDebugMessageCallback(GLDebugCallback, nullptr);

        //glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &MAX_TEXTURES);

        QuadVertex quad[] = {
            {{0.0f, 0.0f}, {0.0f, 0.0f}},
            {{1.0f, 0.0f}, {1.0f, 0.0f}},
            {{0.0f, 1.0f}, {0.0f, 1.0f}},
            {{1.0f, 1.0f}, {1.0f, 1.0f}}
        };

        instances.reserve(MAX_SPRITES);

        unsigned int indices[] =
        {
            0, 1, 2,
            2, 1, 3
        };

        projection = glm::ortho(800.0f, -800.0f, -800.0f, 800.0f);

        int samplers[16];

        for (int i = 0; i < 16; i++)
            samplers[i] = i;

        shader = honse::ResourceManager::Load<honse::Shader>("basic");
        shader->Bind();
        shader->FindUniform("u_Textures").Set(samplers, 16);

        quadVA.Init();
        quadVA.Bind();

        quadVB.Init(quad, sizeof(quad));
        quadEB.Init(indices, sizeof(indices));

        instanceVB.Init(nullptr, MAX_SPRITES * sizeof(SpriteInstance));
        instanceVB.Bind();

        quadVB.Bind();
        quadEB.Bind();

        quadVA.Bind();

        quadVA.AddBuffer(quadVB, {
            { 0, 2, GL_FLOAT },
            { 1, 2, GL_FLOAT }
        });
        
        quadVA.AddBuffer(instanceVB, {
            { 2, 4, GL_FLOAT }, // Tint
            { 3, 2, GL_FLOAT }, // Position
            { 4, 1, GL_FLOAT }, // Rotation
            { 5, 2, GL_FLOAT }, // Scale
            { 6, 2, GL_FLOAT }, // Pivot
            { 7, 2, GL_FLOAT }, // Texture size
            { 8, 1, GL_INT   }  // Texture slot ID
        }, VertexRate::Instance);

        frameQuery = Query(GL_TIME_ELAPSED);
        primitiveQuery = Query(GL_PRIMITIVES_GENERATED);
    }

    int GetTextureSlot(GLuint handle)
    {
        auto it = textureSlotMap.find(handle);

        if (it != textureSlotMap.end())
            return it->second;

        if (textureSlotMap.size() >= MAX_TEXTURES)
        {
            return -1;
        }

        int slot = static_cast<int>(textureSlotMap.size());
        textureSlotMap.emplace(handle, slot);
        textureSlots.push_back(handle); 

        return slot;
    }


};

/////////////////////////////////////////////

void Renderer::OnResolutionChange(glm::vec2 resolution) {
    impl->projection = glm::ortho(0.0f, resolution.x, 0.0f, resolution.y);
    honse::Camera::GetMainCamera()->m_ViewportSize = resolution;
}

void Renderer::Init() {

    impl = std::make_unique<Impl>();
    impl->initRenderData();
}

void Renderer::Shutdown() {

    impl.reset();
}



void Renderer::Submit(Resource<Texture> texture, glm::vec2& position, float rotationRadians, glm::vec2& scale, glm::vec4& tint, glm::vec2& pivot) {

    if (impl->spriteCount >= impl->MAX_SPRITES ||
    impl->textureSlots.size() >= impl->MAX_TEXTURES)
    {
        Flush();
    }
    
    ///// GENERATE INSTANCE DATA

    int slot = impl->GetTextureSlot(texture->GetHandle());

    if (slot == -1)
    {
        Flush();

        impl->textureSlots.clear();
        impl->textureSlotMap.clear();

        slot = impl->GetTextureSlot(texture->GetHandle());
    }

    impl->instances.push_back({
        tint,
        position,
        rotationRadians,
        scale,
        pivot,
        texture->size,
        slot
    });

    impl->spriteCount++;
    
};

void Renderer::Begin() {

    impl->view = honse::Camera::getViewMatrix(); // Get camera view

    honse::Camera::m_Main->m_Viewport = {
        honse::Camera::m_Main->position.x,
        honse::Camera::m_Main->m_Main->position.y,
        honse::Camera::m_Main->m_Main->position.x + honse::Camera::m_Main->m_Main->m_ViewportSize.x,
        honse::Camera::m_Main->m_Main->position.y + honse::Camera::m_Main->m_Main->m_ViewportSize.y
    };

    // Debug timers
    impl->frameQuery.Begin();
    impl->primitiveQuery.Begin();

    // Clear data
    impl->textureSlots.clear();
    impl->textureSlotMap.clear();
    impl->spriteCount = 0;
    impl->instances.clear();

    impl->shader->Bind();

    glm::mat4 vp = impl->projection * impl->view;
    impl->shader->FindUniform("u_ViewProjection").Set(vp);

    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::End() {
    Flush();

    impl->quadVA.Unbind();
    impl->shader->Unbind();

    // End profiling timers
    impl->frameQuery.End();
    impl->primitiveQuery.End();

    // Output profiling data
    honse::Profiling::Set("Rendering (ms)", impl->frameQuery.GetResult() / 1000000.0 );
    honse::Profiling::Set("Primitives", impl->primitiveQuery.GetResult() );
    honse::Profiling::Set("Draw calls", impl->drawCalls);

    impl->drawCalls = 0;
}

void Renderer::Flush() {

    impl->quadVA.Bind();
    impl->quadVB.Bind();
    impl->instanceVB.Bind();
    impl->quadEB.Bind();
    impl->shader->Bind();

    impl->instanceVB.SetData(impl->instances.data(), impl->instances.size() * sizeof(SpriteInstance));
    // Note: EBO and VBO data is already set in init

    for (GLuint i = 0; i < impl->textureSlots.size(); ++i)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, impl->textureSlots[i]);
    }

    glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, impl->spriteCount);

    impl->drawCalls++;
    
    impl->spriteCount = 0;
    impl->instances.clear();
};

