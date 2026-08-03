#pragma once
#include <string>
#include <memory>
#include <glm/glm.hpp>

namespace honse {

    class Texture {

    public:

        glm::vec2 size;
        int channels = 0;

        Texture();
        Texture(const std::string& path);
        ~Texture();

        Texture(const Texture&) = delete;
        Texture& operator=(const Texture&) = delete;

        Texture(Texture&&) noexcept;
        Texture& operator=(Texture&&) noexcept;

        glm::vec2 GetSize();

    private:

        void Bind(unsigned int slot) const;
        unsigned int GetHandle();

        struct Impl;
        std::unique_ptr<Impl> impl;

        friend class Renderer;
    };

}