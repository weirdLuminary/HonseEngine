#pragma once
#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {

private:

    GLuint m_RendererID = 0;

    static GLuint Compile(const std::string& path, GLenum type);
    static bool CheckCompileStatus(GLuint shader);


public:

    GLuint vertexShader = 0;
    GLuint fragmentShader = 0;

    Shader(const std::string& vertexPath, const std::string& fragmentPath);
    Shader(GLuint vertexShader, const std::string& fragmentPath);
    Shader(const std::string& vertexPath, GLuint fragmentShader);
    Shader(GLuint vertexShader, GLuint fragmentShader);

    ~Shader();

    void Bind() const;
    void Unbind() const;

    inline GLuint GetID() { return m_RendererID; }

    int FindUniform(const std::string& name);

    template<typename T>
    void SetUniform(int uniform, const T& value) { static_assert(false); }

    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    Shader(Shader&& other) noexcept
    {
        m_RendererID = other.m_RendererID;
        other.m_RendererID = 0;
    }

    Shader& operator=(Shader&& other) noexcept
    {
        if (this != &other)
        {
            glDeleteProgram(m_RendererID);
            m_RendererID = other.m_RendererID;
            other.m_RendererID = 0;
        }
        return *this;
    }

};

template<>
inline void Shader::SetUniform<float>(int uniform, const float& value) { 
    glUniform1f(uniform, value);
}

template<>
inline void Shader::SetUniform<int>(int uniform, const int& value) { 
    glUniform1i(uniform, value);
}

template<>
inline void Shader::SetUniform<glm::vec2>(int uniform, const glm::vec2& value) { 
    glUniform2f(uniform, value.x, value.y);
}

template<>
inline void Shader::SetUniform<glm::ivec2>(int uniform, const glm::vec<2, int>& value) { 
    glUniform2i(uniform, value.x, value.y);
}

template<>
inline void Shader::SetUniform<glm::vec3>(int uniform, const glm::vec3& value) { 
    glUniform3f(uniform, value.x, value.y, value.z);
}

template<>
inline void Shader::SetUniform<glm::mat4>(int uniform, const glm::mat4& value) { 
    glUniformMatrix4fv(uniform, 1, GL_FALSE, glm::value_ptr(value));
}