#pragma once

#include <honse/VertexBuffer.h>
#include <honse/Shader.h>
#include <honse/VertexArray.h>
#include <honse/IndexBuffer.h>
#include <honse/Texture.h>
#include <honse/SpriteRenderer.h>

#include <memory>

#include <GLFW/glfw3.h>

namespace Rendering {

    struct Context
    {
        GLFWwindow* window = nullptr;

        std::unique_ptr<Shader> shader;
        std::unique_ptr<SpriteRenderer> renderer;

        glm::mat4 projection;
    };

    inline Context gContext;

    void onWindowResize(GLFWwindow* window, int width, int height)
    {
        glViewport(0, 0, width, height);
        gContext.projection = glm::ortho(-(float)width, (float)width, (float)height, -(float)height, -1.0f, 1.0f);
    }

    static void glfw_error_callback(int code, const char* desc)
    {
        fprintf(stderr, "GLFW %d: %s\n", code, desc);
    }

    void Initialize(int width, int height, const std::string& title) {

        gContext.projection = glm::ortho(-(float)width, (float)width, (float)height, -(float)height, -1.0f, 1.0f);

        glfwSetErrorCallback(glfw_error_callback);
        stbi_set_flip_vertically_on_load(true);  

        if (!glfwInit()) return;

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        GLFWwindow* window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
        if (!window)
        {
            printf("Window init fail!");
            return;
        }
        glfwMakeContextCurrent(window);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            printf("GLAD init fail!");
            return;
        }    


        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

        glDebugMessageCallback(
        [](GLenum source, GLenum type, GLuint id, GLenum severity,
        GLsizei length, const GLchar* message, const void*)
        {
            printf("GL: %s\n", message);
        }, nullptr);

        glfwSetWindowSizeCallback(window, onWindowResize);

    };

    Material FromTexture(Texture& texture) {
        return Material { &texture, gContext.shader };
    }

}