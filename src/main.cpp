#include <glad/glad.h>
#include "Resources/ResourceManager.h"
#include <GLFW/glfw3.h>
#include "Renderer/ShaderProgram.h"
#include "Renderer/Texture2D.h"
#include <iostream>
#include <string>
#include <glm/vec2.hpp>

GLfloat points[] = {
    0.0f, 0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f};

GLfloat colors[] = {
    1.f, 0.f, 0.f,
    0.f, 1.f, 0.f,
    0.f, 0.f, 1.f};


GLfloat textureCoords[] = {
    0.5f,
    1.f,
    1.f,
    0.f,
    0.f,
    0.f,
};

glm::ivec2 g_WindowSize(640, 480);

void glfwWindowSizeCallback(GLFWwindow *pWindow, int width, int height)
{
  g_WindowSize.x = width;
  g_WindowSize.y = height;
  glViewport(0, 0, g_WindowSize.x, g_WindowSize.y);
}

void glfwKeyCallback(GLFWwindow *pWindow, int key, int scancode, int action, int mode)
{
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
  {
    glfwSetWindowShouldClose(pWindow, true);
  }
}

int main(int argc, char **argv)
{
  GLFWwindow *pWindow;

  /* Initialize the library */
  if (!glfwInit())
  {
    std::cout << "glfwInit failed" << std::endl;
    return -1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  /* Create a windowed mode window and its OpenGL context */
  pWindow = glfwCreateWindow(g_windowSizeX, g_windowSizeY, "Battlecity", nullptr, nullptr);
  if (!pWindow)
  {
    glfwTerminate();
    return -1;
  }

  glfwSetWindowSizeCallback(pWindow, glfwWindowSizeCallback);
  glfwSetKeyCallback(pWindow, glfwKeyCallback);

  /* Make the window's context current */
  glfwMakeContextCurrent(pWindow);

  if (!gladLoadGL())
  {
    std::cout << "can't load glad" << std::endl;
    return -1;
  }

  std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
  std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

  glClearColor(0, 0, 0, 1);
  {
    ResourceManager resourceManager(argv[0]);
    auto pDefaultShaderProgram = resourceManager.LoadShaders("DefaultShader", "res/shaders/vertex.txt", "res/shaders/fragment.txt");
    if (!pDefaultShaderProgram->isCompiled())
    {
      std::cerr << "Can't create shader program: "
                << "Default Shader" << std::endl;
      return -1;
    }

    auto texture = resourceManager.LoadTexture("DefaultTexture", "res/textures/map_16x16.png");

    GLuint points_vbo = 0;
    glGenBuffers(1, &points_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

    GLuint colors_vbo = 0;
    glGenBuffers(1, &colors_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

    GLuint texture_vbo = 0;
    glGenBuffers(1, &texture_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, texture_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(textureCoords), textureCoords, GL_STATIC_DRAW);

    GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, texture_vbo);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

    pDefaultShaderProgram->use();
    pDefaultShaderProgram->SetInt("tex", 0);
    while (!glfwWindowShouldClose(pWindow))
    {
      /* Render here */
      glClear(GL_COLOR_BUFFER_BIT);

      pDefaultShaderProgram->use();
      glBindVertexArray(vao);
      texture->Bind();
      glDrawArrays(GL_TRIANGLES, 0, 3);

      /* Swap front and back buffers */
      glfwSwapBuffers(pWindow);

      /* Poll for and process events */
      glfwPollEvents();
    }
  }
  glfwTerminate();
  return 0;
}
