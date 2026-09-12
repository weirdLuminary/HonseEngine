#pragma once
#include <glm/glm.hpp>
#include <memory>
#include <string>

namespace honse {

// TBA: create atlas for rendering fonts more effectively

class Fonts {

    public:
        Fonts() = delete;

        static void Init();

        static void LoadFont(std::string path);

    private:
        struct Impl;
        static std::unique_ptr<Impl> impl;

        struct Glyph {

                unsigned int textureID; // ID handle of the glyph texture
                glm::ivec2 size;        // Size of glyph
                glm::ivec2 bearing;     // Offset from baseline to left/top of glyph
                unsigned int advance;   // Offset to advance to next glyph
        };
};

} // namespace honse