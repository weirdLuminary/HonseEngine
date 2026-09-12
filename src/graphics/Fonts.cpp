#include <ft2build.h>
#include FT_FREETYPE_H

#include <glad/glad.h>
#include <honse/fonts/Fonts.h>
#include <stdio.h>

std::unique_ptr<honse::Fonts::Impl> honse::Fonts::impl;

struct honse::Fonts::Impl {
    public:
        FT_Library ft;

        void InitializeLibrary() {
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            if (FT_Init_FreeType(&ft)) {

                printf("Could not init FreeType Library\n");
                return;
            }
        }
};

void honse::Fonts::Init() {
    impl = std::make_unique<Impl>();
    impl->InitializeLibrary();
}