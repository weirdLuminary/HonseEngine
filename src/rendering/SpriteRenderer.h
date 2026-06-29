#pragma once
#include <glm/glm.hpp>
#include "Rendering.h"

class SpriteRenderer {

public:
        SpriteRenderer();
        ~SpriteRenderer();

        void DrawSprite(Material& material, glm::vec2 position, glm::vec2 pivot,
            glm::vec2 size = glm::vec2(10.0f, 10.0f), float rotate = 0.0f, 
            glm::vec3 color = glm::vec3(1.0f));

        inline void SetProjection(glm::mat4* projection) { this->projection = projection; };

private:
        VertexArray  quadVA;
        VertexBuffer quadVB;
        glm::mat4*   projection;

        void initRenderData();


};