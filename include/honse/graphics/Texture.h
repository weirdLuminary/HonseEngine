#pragma once
#include <glad/glad.h>
#include <string>
#include <memory>

namespace hs {

    class Texture {

    public:

        int width = 0;
        int height = 0;
        int channels = 0;

        Texture();
        Texture(const std::string& path);
        ~Texture();

        Texture(const Texture&) = delete;
        Texture& operator=(const Texture&) = delete;

        Texture(Texture&&) noexcept;
        Texture& operator=(Texture&&) noexcept;

    private:

        void Bind(GLenum slot) const;
        unsigned int GetHandle();

        struct Impl;
        std::unique_ptr<Impl> impl;

        friend class Renderer;
    };

}