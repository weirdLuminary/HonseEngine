#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <honse/graphics/Renderer.h>

#include "glwrappers/FrameBuffer.h"
#include "glwrappers/IndexBuffer.h"
#include "glwrappers/QuadVertex.h"
#include "glwrappers/Query.h"
#include "glwrappers/RenderBuffer.h"
#include "glwrappers/SpriteInstance.h"
#include "glwrappers/VertexArray.h"
#include "glwrappers/VertexBuffer.h"

#include <honse/Engine.h>
#include <honse/graphics/Camera.h>
#include <honse/graphics/Shader.h>
#include <honse/graphics/Texture.h>
#include <honse/modules/profiling/Profiling.h>
#include <honse/modules/resources/ResourceManager.h>

#include <array>
#include <chrono>
#include <iostream>

#include <ft2build.h>
#include FT_FREETYPE_H

using namespace honse;

std::unique_ptr<honse::Renderer::Impl> honse::Renderer::impl;

void GLAPIENTRY GLDebugCallback(GLenum source, GLenum type, GLuint id,
                                GLenum severity, GLsizei length,
                                const GLchar* message, const void* userParam) {
    printf("[GL DEBUG] %s\n", message);
}

struct Renderer::Impl {

    Query frameQuery;           // Query for profiling rendering time
    Query primitiveQuery;       // Query for profiling primitives drawn
    unsigned int drawCalls = 0; // Draw calls this frame

    VertexBuffer quadVB; // Base quad VBO
    IndexBuffer quadEB;  // Base quad EBO

    glm::vec2 resolution;

    ////////////////        Main rendering      ////////////////

    VertexArray quadVA;      // Batch renderer VAO
    VertexBuffer instanceVB; // Batch renderer instance VBO

    glm::mat4 projection; //  Projection & View matrices
    glm::mat4 view;       //  respectively

    std::vector<SpriteInstance> instances; // Per-sprite settings

    std::unordered_map<GLuint, int>
        textureSlotMap;               // Lookup map for texture slots
    std::vector<GLuint> textureSlots; // Actual texture slot list

    Resource<Shader> shader; // Shader for batch rendering

    unsigned int spriteCount = 0;     // Sprite count this frame
    const size_t MAX_SPRITES = 10000; // Max amount of sprites per frame
    GLint MAX_TEXTURES =
        16; // Max amount of loaded textures before Flush is called

    ////////////////        Post-processing     ////////////////

    VertexArray postProcVA; // Post-processing VAO

    VertexBuffer postProcQuadVB; // Post-processing frame quad VBO
    std::array<FrameBuffer, 2>
        frameBuffers;              // Intermediate ping-pong framebuffers
    FrameBuffer multisampleBuffer; //
    RenderBuffer renderBuffer;     //

    std::vector<Resource<Shader>>
        postProcessingShaders; // Post-processing shaders

    void initRenderData() {

        printf("%s\n\n", glGetString(GL_VERSION));

        // glEnable(GL_DEBUG_OUTPUT);
        // glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        // glDebugMessageCallback(GLDebugCallback, nullptr);
        // glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0,
        // nullptr, GL_TRUE);

        glEnable(GL_BLEND);
        glEnable(GL_MULTISAMPLE);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glDisable(GL_STENCIL_TEST);
        glDisable(GL_SCISSOR_TEST);
        glDisable(GL_CULL_FACE);
        glDisable(GL_RASTERIZER_DISCARD);

        //////////////      Batch renderer      //////////////

        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

        QuadVertex quad[] = {{{0.0f, 0.0f}, {0.0f, 0.0f}},
                             {{1.0f, 0.0f}, {1.0f, 0.0f}},
                             {{0.0f, 1.0f}, {0.0f, 1.0f}},
                             {{1.0f, 1.0f}, {1.0f, 1.0f}}};

        QuadVertex postProcQuad[] = {{{-1.0f, -1.0f}, {0.0f, 0.0f}},
                                     {{1.0f, -1.0f}, {1.0f, 0.0f}},
                                     {{-1.0f, 1.0f}, {0.0f, 1.0f}},
                                     {{1.0f, 1.0f}, {1.0f, 1.0f}}};

        unsigned int indices[] = {0, 1, 2, 2, 1, 3};

        projection = glm::ortho(800.0f, -800.0f, -800.0f,
                                800.0f); // Create default projection; temporary

        int samplers[16];

        for (int i = 0; i < 16; i++)
            samplers[i] = i;

        shader = honse::ResourceManager::Load<honse::Shader>("default");
        shader->Bind();
        shader->Set("u_Textures", samplers, 16);

        quadVA = VertexArray();
        quadVA.Bind();

        quadVB = VertexBuffer(quad, sizeof(quad));
        quadEB = IndexBuffer(indices, sizeof(indices));

        instanceVB =
            VertexBuffer(nullptr, MAX_SPRITES * sizeof(SpriteInstance));
        instanceVB.Bind();

        quadVB.Bind();
        quadEB.Bind();
        quadVA.Bind();

        quadVA.AddBuffer(quadVB, {
                                     {0, 2, GL_FLOAT}, // Individual position
                                     {1, 2, GL_FLOAT}  // UV
                                 });

        quadVA.AddBuffer(instanceVB,
                         {
                             {2, 4, GL_FLOAT}, // Tint
                             {3, 2, GL_FLOAT}, // Position
                             {4, 1, GL_FLOAT}, // Rotation
                             {5, 2, GL_FLOAT}, // Scale
                             {6, 2, GL_FLOAT}, // Pivot
                             {7, 2, GL_FLOAT}, // Texture size
                             {8, 1, GL_INT}    // Texture slot ID
                         },
                         VertexRate::Instance);

        instances.reserve(MAX_SPRITES);

        //////////////     Post-processing     //////////////

        postProcVA.Bind();

        postProcQuadVB = VertexBuffer(postProcQuad, sizeof(postProcQuad));

        multisampleBuffer = FrameBuffer(true);

        frameBuffers[1].Bind();
        frameBuffers[1].AttachTexture(800, 800);

        frameBuffers[0].Bind();
        frameBuffers[0].AttachTexture(800, 800);

        multisampleBuffer.Bind();
        multisampleBuffer.AttachTexture(800, 800);

        renderBuffer = RenderBuffer(800, 800);
        renderBuffer.Bind();
        renderBuffer.AttachFramebuffer();

        renderBuffer.Unbind();

        FrameBuffer::Unbind();

        postProcVA.AddBuffer(postProcQuadVB,
                             {
                                 {0, 2, GL_FLOAT}, // Individual position
                                 {1, 2, GL_FLOAT}  // UV
                             });

        postProcVA.Unbind();

        //////////////         Profiling       //////////////

        frameQuery = Query(GL_TIME_ELAPSED);
        primitiveQuery = Query(GL_PRIMITIVES_GENERATED);
    }

    void Clear() {
        multisampleBuffer.Bind();
        glClear(GL_COLOR_BUFFER_BIT);

        FrameBuffer::Unbind();
        glClear(GL_COLOR_BUFFER_BIT);

        if (IsPostProcessingEnabled())
            multisampleBuffer.Bind();
    }

    bool IsPostProcessingEnabled() { return !postProcessingShaders.empty(); }

    void ApplyPostProcessing() {

        impl->postProcVA.Bind();
        impl->quadEB.Bind();

        impl->multisampleBuffer.Bind(true, false);
        impl->frameBuffers[0].Bind(false, true);

        impl->multisampleBuffer.Blit(impl->resolution.x, impl->resolution.y);

        impl->frameBuffers[0].Bind(true, false);

        FrameBuffer::Unbind();

        GLuint inputTex = impl->frameBuffers[0].GetTexture();

        int inputBuffer = 0;
        int outputBuffer = 1;

        for (size_t i = 0; i < impl->postProcessingShaders.size(); i++) {
            auto& shader = impl->postProcessingShaders[i];

            const bool last = i == impl->postProcessingShaders.size() - 1;

            shader->Bind();

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, inputTex);

            if (last) {
                FrameBuffer::Unbind();
            }
            else {
                impl->frameBuffers[outputBuffer].Bind(false, true);
            }

            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

            shader->Unbind();

            if (!last) {
                inputBuffer = outputBuffer;
                outputBuffer = 1 - outputBuffer;

                inputTex = impl->frameBuffers[inputBuffer].GetTexture();
            }
        }

        impl->quadEB.Unbind();
        impl->postProcVA.Unbind();
    }

    int GetTextureSlot(GLuint handle) {
        auto it = textureSlotMap.find(handle);

        if (it != textureSlotMap.end())
            return it->second;

        if (textureSlotMap.size() >= MAX_TEXTURES) {
            return -1;
        }

        int slot = static_cast<int>(textureSlotMap.size());
        textureSlotMap.emplace(handle, slot);
        textureSlots.push_back(handle);

        return slot;
    }
};

/////////////////////////////////////////////

void Renderer::AddPostProcessingShader(Resource<Shader> shader) {
    impl->postProcessingShaders.push_back(shader);
    shader->Bind();
    shader->Set("screenTexture", 0);
    shader->Unbind();
}

void Renderer::OnResolutionChange(glm::vec2 resolution) {
    auto& w = resolution.x;
    auto& h = resolution.y;

    impl->projection = glm::ortho(0.0f, w, 0.0f, h);

    impl->frameBuffers[0].ResizeTexture(w, h);
    impl->frameBuffers[1].ResizeTexture(w, h);

    impl->multisampleBuffer.ResizeTexture(w, h);

    impl->renderBuffer.Bind();
    glViewport(0, 0, w, h);
    impl->renderBuffer.Resize(w, h);
    impl->renderBuffer.Unbind();

    honse::Camera::GetMainCamera()->m_ViewportSize = resolution;
    impl->resolution = resolution;
}

void Renderer::Init() {

    impl = std::make_unique<Impl>();
    impl->initRenderData();
}

void Renderer::Shutdown() { impl.reset(); }

void Renderer::Submit(Resource<Texture> texture, glm::vec2& position,
                      float rotationRadians, glm::vec2& scale, glm::vec4& tint,
                      glm::vec2& pivot) {

    if (impl->spriteCount >= impl->MAX_SPRITES ||
        impl->textureSlots.size() >= impl->MAX_TEXTURES) {
        Flush();
    }

    ///// GENERATE INSTANCE DATA

    int slot = impl->GetTextureSlot(texture->GetHandle());

    if (slot == -1) {
        Flush();

        impl->textureSlots.clear();
        impl->textureSlotMap.clear();

        slot = impl->GetTextureSlot(texture->GetHandle());
    }

    impl->instances.push_back(
        {tint, position, rotationRadians, scale, pivot, texture->size, slot});

    impl->spriteCount++;
};

void Renderer::Begin() {

    impl->view = honse::Camera::getViewMatrix(); // Get camera view

    honse::Camera::m_Main->m_Viewport = {
        honse::Camera::m_Main->position.x,
        honse::Camera::m_Main->m_Main->position.y,
        honse::Camera::m_Main->m_Main->position.x +
            honse::Camera::m_Main->m_Main->m_ViewportSize.x,
        honse::Camera::m_Main->m_Main->position.y +
            honse::Camera::m_Main->m_Main->m_ViewportSize.y};

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
    impl->shader->Set("u_ViewProjection", vp);

    impl->Clear();
}

void Renderer::End() {
    Flush();

    if (impl->IsPostProcessingEnabled()) {
        impl->shader->Unbind();
        impl->quadVA.Unbind();
        impl->quadEB.Unbind();

        impl->ApplyPostProcessing();
    }
    else {

        impl->quadVA.Unbind();
        impl->quadEB.Unbind();
        impl->shader->Unbind();
    }

    // End profiling timers
    impl->frameQuery.End();
    impl->primitiveQuery.End();

    // Output profiling data
    honse::Profiling::Set("Rendering (ms)",
                          impl->frameQuery.GetResult() / 1000000.0);
    honse::Profiling::Set("Primitives", impl->primitiveQuery.GetResult());
    honse::Profiling::Set("Draw calls", impl->drawCalls);

    impl->drawCalls = 0;
}

void Renderer::Flush() {

    impl->quadVA.Bind();
    impl->quadEB.Bind();
    impl->shader->Bind();

    impl->instanceVB.SetData(impl->instances.data(),
                             impl->instances.size() * sizeof(SpriteInstance));
    // Note: EBO and VBO data is already set in init

    for (GLuint i = 0; i < impl->textureSlots.size(); ++i) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, impl->textureSlots[i]);
    }

    glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0,
                            impl->spriteCount);

    impl->spriteCount = 0;
    impl->instances.clear();

    impl->drawCalls++;
};
