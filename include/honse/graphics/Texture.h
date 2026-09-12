#pragma once
#include <glm/glm.hpp>
#include <memory>
#include <string>

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

    private:
        void Bind(unsigned int slot) const;
        unsigned int GetHandle();

        struct Impl;
        std::unique_ptr<Impl> impl;

        friend class Renderer;
};

} // namespace honse