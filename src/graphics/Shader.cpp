#include <glad/glad.h>
#include <honse/graphics/Shader.h>
#include <honse/modules/File.h>

using namespace honse;

///// UNIFORM /////

void Shader::Set(const std::string& name, float value) {
    int uniform = glGetUniformLocation(m_RendererID, name.c_str());
    if (uniform == -1)
        printf("Uniform %s not found! \n", name.c_str());
    Bind();
    glUniform1f(uniform, value);
}

void Shader::Set(const std::string& name, int value) {
    int uniform = glGetUniformLocation(m_RendererID, name.c_str());
    if (uniform == -1)
        printf("Uniform %s not found! \n", name.c_str());
    Bind();
    glUniform1i(uniform, value);
}

void Shader::Set(const std::string& name, const glm::vec2& value) {
    int uniform = glGetUniformLocation(m_RendererID, name.c_str());
    if (uniform == -1)
        printf("Uniform %s not found! \n", name.c_str());
    Bind();
    glUniform2f(uniform, value.x, value.y);
}

void Shader::Set(const std::string& name, const glm::ivec2& value) {
    int uniform = glGetUniformLocation(m_RendererID, name.c_str());
    if (uniform == -1)
        printf("Uniform %s not found! \n", name.c_str());
    Bind();
    glUniform2i(uniform, value.x, value.y);
}

void Shader::Set(const std::string& name, const glm::vec3& value) {
    int uniform = glGetUniformLocation(m_RendererID, name.c_str());
    if (uniform == -1)
        printf("Uniform %s not found! \n", name.c_str());
    Bind();
    glUniform3f(uniform, value.x, value.y, value.z);
}

void Shader::Set(const std::string& name, const glm::vec4& value) {
    int uniform = glGetUniformLocation(m_RendererID, name.c_str());
    if (uniform == -1)
        printf("Uniform %s not found! \n", name.c_str());
    Bind();
    glUniform4f(uniform, value.x, value.y, value.z, value.w);
}

void Shader::Set(const std::string& name, const int* values, int count) {
    int uniform = glGetUniformLocation(m_RendererID, name.c_str());
    if (uniform == -1)
        printf("Uniform %s not found! \n", name.c_str());
    Bind();
    glUniform1iv(uniform, count, values);
}

void Shader::Set(const std::string& name, const glm::mat4& value) {
    int uniform = glGetUniformLocation(m_RendererID, name.c_str());
    if (uniform == -1)
        printf("Uniform %s not found! \n", name.c_str());
    Bind();
    glUniformMatrix4fv(uniform, 1, GL_FALSE, glm::value_ptr(value));
};

///// SHADER /////

bool Shader::CheckCompileStatus(unsigned int shader) {
    int compileStatus;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
    if (compileStatus != GL_TRUE) {

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

bool Shader::CheckLinkingStatus(unsigned int program) {
    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char log[1024];
        glGetProgramInfoLog(program, 1024, nullptr, log);
        printf("Shader link failed:\n%s\n", log);
    }
    return success;
}

unsigned int Shader::Compile(const std::string& source, int type) {

    GLuint id = 0;

    const char* src = source.c_str();

    id = glCreateShader(type);
    glShaderSource(id, 1, &src, 0);
    glCompileShader(id);
    if (!CheckCompileStatus(id)) {
        printf("Shader %d failed to compile! \n", id);
        return 0;
    }

    return id;
}

void Shader::AttachShaders() {
    glAttachShader(m_RendererID, vertexShader);
    glAttachShader(m_RendererID, fragmentShader);
    glLinkProgram(m_RendererID);

    CheckLinkingStatus(m_RendererID);

    glDetachShader(m_RendererID, vertexShader);
    glDetachShader(m_RendererID, fragmentShader);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Shader::Shader(std::string fragmentPath, bool postProcessing)
    : postProcessing(postProcessing) {

    m_RendererID = glCreateProgram();
    printf("Created shader program %d!\n", m_RendererID);

    File fragFile(fragmentPath);

    vertexShader = Compile(postProcessing ? basicVertexSrc : defaultVertexSrc,
                           GL_VERTEX_SHADER);
    fragmentShader = Compile(fragFile.Read(), GL_FRAGMENT_SHADER);

    AttachShaders();
}

Shader::Shader() {

    m_RendererID = glCreateProgram();
    printf("Created shader program %d!\n", m_RendererID);

    vertexShader = Compile(defaultVertexSrc, GL_VERTEX_SHADER);
    fragmentShader = Compile(defaultFragmentSrc, GL_FRAGMENT_SHADER);

    AttachShaders();
}

Shader::Shader(std::string vertexPath, std::string fragmentPath) {

    m_RendererID = glCreateProgram();
    printf("Created shader program %d!\n", m_RendererID);

    File vertFile(vertexPath);
    File fragFile(fragmentPath);

    vertexShader = Compile(vertFile.Read(), GL_VERTEX_SHADER);
    fragmentShader = Compile(fragFile.Read(), GL_FRAGMENT_SHADER);

    AttachShaders();
}

Shader::~Shader() { glDeleteProgram(m_RendererID); }

Shader& Shader::operator=(Shader&& other) noexcept {
    if (this != &other) {
        glDeleteProgram(m_RendererID);
        m_RendererID = other.m_RendererID;
        other.m_RendererID = 0;
    }
    return *this;
}

void Shader::Bind() const { glUseProgram(m_RendererID); }

void Shader::Unbind() const { glUseProgram(0); }
