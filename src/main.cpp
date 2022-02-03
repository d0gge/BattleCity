#include "Renderer/ShaderProgram.h"
#include "Renderer/Sprite.h"
#include "Renderer/Texture2D.h"
#include "Resources/ResourceManager.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <iostream>
#include <string>

GLfloat points[] = {0.0f, 50.f, 0.0f, 50.f, -50.f, 0.0f, -50.f, -50.f, 0.0f};

GLfloat colors[] = {1.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 1.f};

GLfloat textureCoords[] = {
  0.5f, 1.f, 1.f, 0.f, 0.f, 0.f,
};

glm::ivec2 g_WindowSize(640, 480);

void glfwWindowSizeCallback(GLFWwindow* pWindow, int width, int height)
{
  g_WindowSize.x = width;
  g_WindowSize.y = height;
  glViewport(0, 0, g_WindowSize.x, g_WindowSize.y);
}

void glfwKeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mode)
{
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
  {
    glfwSetWindowShouldClose(pWindow, true);
  }
}

int main(int argc, char** argv)
{
  GLFWwindow* pWindow;

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
  pWindow = glfwCreateWindow(g_WindowSize.x, g_WindowSize.y, "Battlecity", nullptr, nullptr);
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
    auto pDefaultShaderProgram =
      resourceManager.LoadShaders("DefaultShader", "res/shaders/vertex.txt", "res/shaders/fragment.txt");
    if (!pDefaultShaderProgram->isCompiled())
    {
      std::cerr << "Can't create shader program: "
                << "Default Shader" << std::endl;
      return -1;
    }

    auto pSpriteShaderProgram =
      resourceManager.LoadShaders("SpriteShader", "res/shaders/vSprite.txt", "res/shaders/fSprite.txt");
    if (!pSpriteShaderProgram->isCompiled())
    {
      std::cerr << "Can't create shader program: "
                << "Default Shader" << std::endl;
      return -1;
    }
    auto texture = resourceManager.LoadTexture("DefaultTexture", "res/textures/map_16x16.png");
    auto pSprite = resourceManager.LoadSprite("NewSprite", "DefaultTexture", "SpriteShader", 50, 100);
    pSprite->SetPosition(glm::vec2(200, 100));

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

    glm::mat4 modelMatrix_1 = glm::mat4(1.f);
    modelMatrix_1 = glm::translate(modelMatrix_1, glm::vec3(50.f, 50.f, 0.f));

    glm::mat4 modelMatrix_2 = glm::mat4(1.f);
    modelMatrix_2 = glm::translate(modelMatrix_2, glm::vec3(590.f, 50.f, 0.f));

    glm::mat4 projectionMatrix =
      glm::ortho(0.f, static_cast<float>(g_WindowSize.x), 0.f, static_cast<float>(g_WindowSize.y), -100.f, 100.f);

    pDefaultShaderProgram->SetMatrix4("projectionMatrix", projectionMatrix);
    pSpriteShaderProgram->use();
    pSpriteShaderProgram->SetInt("tex", 0);
    pSpriteShaderProgram->SetMatrix4("projectionMatrix", projectionMatrix);

    while (!glfwWindowShouldClose(pWindow))
    {
      /* Render here */
      glClear(GL_COLOR_BUFFER_BIT);

      pDefaultShaderProgram->use();
      glBindVertexArray(vao);
      texture->Bind();

      pDefaultShaderProgram->SetMatrix4("modelMatrix", modelMatrix_1);
      glDrawArrays(GL_TRIANGLES, 0, 3);

      pDefaultShaderProgram->SetMatrix4("modelMatrix", modelMatrix_2);
      glDrawArrays(GL_TRIANGLES, 0, 3);

      pSprite->Render();
      /* Swap front and back buffers */
      glfwSwapBuffers(pWindow);

      /* Poll for and process events */
      glfwPollEvents();
    }
  }
  glfwTerminate();
  return 0;
}
