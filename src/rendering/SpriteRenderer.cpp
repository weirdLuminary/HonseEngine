#include "SpriteRenderer.h"
#include <iostream>

SpriteRenderer::SpriteRenderer() {

    initRenderData();
}

SpriteRenderer::~SpriteRenderer() {
}

void SpriteRenderer::initRenderData()
{
    // configure VAO/VBO

    float vertices[] = { 
        // pos      // tex
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 
    
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };

    

    quadVA = VertexArray();
    quadVB = VertexBuffer(vertices, sizeof(vertices));

    quadVA.AddBuffer(quadVB, {
        { 2, GL_FLOAT },
        { 2, GL_FLOAT }
    });

    quadVB.Unbind();
    quadVA.Unbind();
}

void SpriteRenderer::DrawSprite(Material& material, glm::vec2 position, glm::vec2 pivot, glm::vec2 size, float rotate, glm::vec3 color) {

    auto* shader = material.shader;

    shader->Bind();

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position - pivot * size, 0.0f));  

    model = glm::translate(model, glm::vec3(pivot.x * size.x, pivot.y * size.y, 0.0f)); 
    model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f)); 
    model = glm::translate(model, glm::vec3(-pivot.x * size.x, -pivot.y * size.y, 0.0f));

    model = glm::scale(model, glm::vec3(size, 1.0f)); 
    
    shader->SetUniform(shader->FindUniform("model"), model);
    shader->SetUniform(shader->FindUniform("projection"), *projection);
    shader->SetUniform(shader->FindUniform("spriteColor"), color);

    material.texture->Bind(GL_TEXTURE0);
    shader->SetUniform(shader->FindUniform("image"), 0);
    quadVA.Bind();

    glDrawArrays(GL_TRIANGLES, 0, 6);

    quadVA.Unbind();
}