#pragma once
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace hs {
    


    struct Uniform {

        int ID;

        void Set(int value);
        void Set(float value);
        void Set(const glm::ivec2& value);
        void Set(const glm::vec2& value);
        void Set(const glm::ivec3& value);
        void Set(const glm::vec3& value);
        void Set(const glm::mat4& value);
        void Set(const glm::vec4& value);
        void Set(const int* values, int count);

    };

    class Shader {

    private:

        unsigned int m_RendererID = 0;

        unsigned int vertexShader = 0;
        unsigned int fragmentShader = 0;

        const char* defaultVertexSrc = R"(#version 330 core
            layout(location = 0) in vec4 a_Position;
            layout(location = 1) in vec2 a_TexCoord;
            layout(location = 3) in int  a_TextureID;
            layout(location = 2) in vec4 a_Color;

            out vec2 v_TexCoord;
            flat out int v_TextureID;
            out vec4 v_Color;

            void main()
            {
                gl_Position = a_Position;
                v_TexCoord = a_TexCoord;
                v_TextureID = a_TextureID;
                v_Color = a_Color;
            }
            )";

        const char* defaultFragmentSrc = R"(#version 330 core
            in vec2 v_TexCoord;
            flat in int v_TextureID;
            in vec4 v_Color;

            out vec4 FragColor;

            uniform sampler2D u_Textures[16];

            void main()
            {
                FragColor = texture(u_Textures[v_TextureID], v_TexCoord) * v_Color;
            }
            )";

        static unsigned int Compile(const std::string& path, int type);
        static bool CheckCompileStatus(unsigned int shader);

        void Bind() const;
        void Unbind() const;

        friend class Renderer;

    public:

        Shader FromSource(const std::string& vertexSrc, const std::string& fragSrc);    // For custom vertex shader
        Shader FromSource(const std::string& fragSrc);                                  // For default vertex shader

        Shader(const std::string& vertexPath, const std::string& fragmentPath);         // For custom vertex shader
        Shader(const std::string& fragmentPath);                                        // For default vertex shader
        Shader();                                                                       // Default implementation for both shaders
        ~Shader();

        Uniform FindUniform(const std::string& name);

        Shader(const Shader&) = delete;
        Shader& operator=(const Shader&) = delete;

        Shader(Shader&& other) noexcept
        {
            m_RendererID = other.m_RendererID;
            other.m_RendererID = 0;
        }

        Shader& operator=(Shader&& other) noexcept;

    };

   


}