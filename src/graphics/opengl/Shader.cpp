#include <honse/graphics/Shader.h>
#include <honse/modules/File.h>
#include <glad/glad.h>

using namespace hs;

///// UNIFORM /////

void Uniform::Set(float value) { 
    glUniform1f(ID, value);
}

void Uniform::Set(int value) { 
    glUniform1i(ID, value);
}

void Uniform::Set(const glm::vec2& value) { 
    glUniform2f(ID, value.x, value.y);
}

void Uniform::Set(const glm::ivec2& value) { 
    glUniform2i(ID, value.x, value.y);
}

void Uniform::Set(const glm::vec3& value) { 
    glUniform3f(ID, value.x, value.y, value.z);
}

void Uniform::Set(const glm::vec4& value) { 
    glUniform4f(ID, value.x, value.y, value.z, value.w);
}

void Uniform::Set(const int* values, int count) {
    glUniform1iv(ID, count, values);
}

void Uniform::Set(const glm::mat4& value) { 
    glUniformMatrix4fv(ID, 1, GL_FALSE, glm::value_ptr(value));
};

///// SHADER /////

bool Shader::CheckCompileStatus(GLuint shader) {
    int compileStatus;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
    if(compileStatus != GL_TRUE) {

        GLsizei log_length = 0;
        char message[1024];
        glGetShaderInfoLog(shader, 1024, &log_length, message);
        printf("%s\n", message);

        glDeleteShader(shader);

        return false;
    }

    printf("Shader %u compiled!\n", shader);
    

    return true;
}


unsigned int Shader::Compile(const std::string& source, int type) {

    GLuint id = 0;

    const char* src = source.c_str();

    id = glCreateShader(type);
    glShaderSource(id, 1, &src, 0);
    glCompileShader(id);
    if(!CheckCompileStatus(id)) { 
        printf("Shader %d failed to compile! \n", id);
        return 0;
    }

    return id;
}

Shader::Shader(const std::string& fragmentPath) {

    m_RendererID = glCreateProgram();
    printf("Created shader program %d!\n", m_RendererID);

    vertexShader = Compile(defaultVertexSrc, GL_VERTEX_SHADER);
    fragmentShader = Compile(fragmentPath, GL_FRAGMENT_SHADER);

    glAttachShader(m_RendererID, vertexShader);
    glAttachShader(m_RendererID, fragmentShader);
    glLinkProgram(m_RendererID);

    GLint success;
    glGetProgramiv(m_RendererID, GL_LINK_STATUS, &success);
    if (!success)
    {
        char log[1024];
        glGetProgramInfoLog(m_RendererID, 1024, nullptr, log);
        printf("Shader link failed:\n%s\n", log);
    }

    glDetachShader(m_RendererID, vertexShader);
    glDetachShader(m_RendererID, fragmentShader);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

}

Shader::Shader() {

    m_RendererID = glCreateProgram();
    printf("Created shader program %d!\n", m_RendererID);


    vertexShader = Compile(defaultVertexSrc, GL_VERTEX_SHADER);
    fragmentShader = Compile(defaultFragmentSrc, GL_FRAGMENT_SHADER);

    glAttachShader(m_RendererID, vertexShader);
    glAttachShader(m_RendererID, fragmentShader);
    glLinkProgram(m_RendererID);

    glDetachShader(m_RendererID, vertexShader);
    glDetachShader(m_RendererID, fragmentShader);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

}


Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath) {

    m_RendererID = glCreateProgram();
    printf("Created shader program %d!\n", m_RendererID);

    File vertFile(vertexPath);
    File fragFile(fragmentPath);

    vertexShader = Compile(vertFile.Read(), GL_VERTEX_SHADER);
    fragmentShader = Compile(fragFile.Read(), GL_FRAGMENT_SHADER);

    glAttachShader(m_RendererID, vertexShader);
    glAttachShader(m_RendererID, fragmentShader);
    glLinkProgram(m_RendererID);

    glDetachShader(m_RendererID, vertexShader);
    glDetachShader(m_RendererID, fragmentShader);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

}

Shader::~Shader() {

    glDeleteProgram(m_RendererID);

}

Shader& Shader::operator=(Shader&& other) noexcept
{
    if (this != &other)
    {
        glDeleteProgram(m_RendererID);
        m_RendererID = other.m_RendererID;
        other.m_RendererID = 0;
    }
    return *this;
}

void Shader::Bind() const {

    glUseProgram(m_RendererID);

}

void Shader::Unbind() const {

    glUseProgram(0);

}

Uniform Shader::FindUniform(const std::string& name) {
    int uniform = glGetUniformLocation(m_RendererID, name.c_str());
    if(uniform == -1) printf("Uniform %s not found! \n", name.c_str());

    return (Uniform){ uniform };
}

