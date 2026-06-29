#include "Shader.h"
#include <fstream>
#include <sstream>
#include <stdio.h>

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

GLuint Shader::Compile(const std::string& path, GLenum type) {

    GLuint id = 0;

    std::ifstream file(path);

    std::string line;
    std::stringstream ss;
    if(file.is_open()) {
        while(std::getline(file, line)) 
            ss << line << "\n";
    
        std::string source = ss.str();
        const char* src = source.c_str();
        const char* adapter[1] { src };

        id = glCreateShader(type);
        glShaderSource(id, 1, adapter, 0);
        glCompileShader(id);
        if(!CheckCompileStatus(id)) { 
            printf("Shader %d failed to compile! \n", id);
            return 0;
        }
    }

    return id;
}




Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath) {

    m_RendererID = glCreateProgram();
    printf("Created shader program %d!\n", m_RendererID);

    vertexShader = Compile(vertexPath, GL_VERTEX_SHADER);
    fragmentShader = Compile(fragmentPath, GL_FRAGMENT_SHADER);

    glAttachShader(m_RendererID, vertexShader);
    glAttachShader(m_RendererID, fragmentShader);
    glLinkProgram(m_RendererID);

    glDetachShader(m_RendererID, vertexShader);
    glDetachShader(m_RendererID, fragmentShader);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

}

Shader::Shader(GLuint vertex, const std::string& fragmentPath) {

    m_RendererID = glCreateProgram();
    printf("Created shader program %d!\n", m_RendererID);

    vertexShader = vertex;
    fragmentShader = Compile(fragmentPath, GL_FRAGMENT_SHADER);

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

void Shader::Bind() const {

    glUseProgram(m_RendererID);

}

void Shader::Unbind() const {

    glUseProgram(0);

}

int Shader::FindUniform(const std::string& name) {
    int uniform = glGetUniformLocation(m_RendererID, name.c_str());
    if(uniform == -1) printf("Uniform %s not found! \n", name.c_str());

    return uniform;
}

