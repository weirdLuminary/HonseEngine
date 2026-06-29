#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "rendering/SpriteRenderer.h"
#include <stb_image.h>
#include <iostream>

SpriteRenderer *renderer;
glm::mat4 projection;

void onWindowResize(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    projection = glm::ortho(-(float)width, (float)width, (float)height, -(float)height, -1.0f, 1.0f);
}

static void glfw_error_callback(int code, const char* desc)
{
    fprintf(stderr, "GLFW %d: %s\n", code, desc);
}



int main()
{

    int width = 640;
    int height = 640;

    projection = glm::ortho(-(float)width, (float)width, (float)height, -(float)height, -1.0f, 1.0f);

    glfwSetErrorCallback(glfw_error_callback);
    stbi_set_flip_vertically_on_load(true);  

    if (!glfwInit()) return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(width, height, "My Title", NULL, NULL);
    if (!window)
    {
        printf("Window init fail!");
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("GLAD init fail!");
        return -1;
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

    //////////////////////////////////////////

    Texture tex("res/container2.png");
    Texture tex2("res/container2glow.png");

    Shader shader("res/sprite.glsl", "res/spritef.glsl");

    Material base = Material { &tex, &shader };

    renderer = new SpriteRenderer();
    renderer->SetProjection(&projection);

    tex2.Bind(GL_TEXTURE1);

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        glfwGetWindowSize(window, &width, &height);

        renderer->DrawSprite(base, glm::vec2(0.0f), glm::vec2(0.5f), glm::vec2(500.0f), glfwGetTime() * 10.0f, glm::vec3(1.0f));
 
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    delete renderer;

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}