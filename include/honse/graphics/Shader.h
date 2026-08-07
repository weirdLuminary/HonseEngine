#pragma once
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace honse {
    


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
            layout(location = 0) in vec2 a_Position;
            layout(location = 1) in vec2 a_UV;

            layout(location = 2) in vec4 a_Tint;
            layout(location = 3) in vec2 a_PositionInstance;
            layout(location = 4) in float a_Rotation;
            layout(location = 5) in vec2 a_Scale;
            layout(location = 6) in vec2 a_Pivot;
            layout(location = 7) in vec2 a_Size;
            layout(location = 8) in int a_TextureSlot;

            uniform mat4 u_ViewProjection;

            out vec2 v_TexCoord;
            out vec4 v_Color;
            flat out int v_TextureSlot;

            void main()
            {
                vec2 p = a_Position - a_Pivot;

                p *= a_Scale * a_Size;

                float c = cos(a_Rotation);
                float s = sin(a_Rotation);

                p = vec2(
                    c * p.x - s * p.y,
                    s * p.x + c * p.y
                );

                p += a_PositionInstance;

                gl_Position = u_ViewProjection * vec4(p, 0.0, 1.0);

                v_TexCoord = a_UV;
                v_Color = a_Tint;
                v_TextureSlot = a_TextureSlot;
            }
            )";

        const char* defaultFragmentSrc = R"(#version 330 core
            in vec2 v_TexCoord;
            flat in int v_TextureSlot;
            in vec4 v_Color;

            out vec4 FragColor;

            uniform sampler2D u_Textures[16];

            void main()
            {
                FragColor = texture(u_Textures[v_TextureSlot], v_TexCoord) * v_Color;
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

        Shader(std::string& vertexPath, std::string& fragmentPath);         // For custom vertex shader
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