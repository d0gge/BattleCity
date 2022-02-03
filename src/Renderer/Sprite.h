#pragma once
#include <glad/glad.h>
#include <memory>
#include <glm/vec2.hpp>

namespace Renderer
{
  class Texture2D;
  class ShaderProgram;

  class Sprite
  {
  public:
    Sprite(const std::shared_ptr<Texture2D> pTexture,
           const std::shared_ptr<ShaderProgram> pShaderProgram,
           const glm::vec2& position = glm::vec2(0.f), 
           const glm::vec2& size = glm::vec2(1.f),
           const float rotation = 0.f);
    ~Sprite();

    Sprite(const Sprite&) = delete;
    Sprite& operator=(const Sprite&) = delete;

    void Render() const;
    void SetPosition(const glm::vec2& position); 
    void SetSize(const glm::vec2& size);
    void SetRotation(const float rotation);

  private:
    std::shared_ptr<Texture2D> m_pTexture;
    std::shared_ptr<ShaderProgram> m_pShaderProgram;
    glm::vec2 m_Position;
    glm::vec2 m_Size;
    float m_Rotation;
    GLuint m_Vao;
    GLuint m_VertexCoordsVbo;
    GLuint m_TextureCoordsVbo;
  };
}