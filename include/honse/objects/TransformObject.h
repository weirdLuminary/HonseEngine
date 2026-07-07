#pragma once
#include "Object.h"

namespace hs {

    class TransformObject : public Object {

        public:

            glm::vec2 position = glm::vec2(0.0f);
            glm::vec2 pivot = glm::vec2(0.5f);
            float rotation = 0.0f;

    };

}