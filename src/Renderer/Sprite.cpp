#include "Sprite.h"
#include "ShaderProgram.h"
#include "Texture2D.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>

namespace Renderer
{
Sprite::Sprite(const std::shared_ptr<Texture2D> pTexture, const std::shared_ptr<ShaderProgram> pShaderProgram,
               const glm::vec2& position, const glm::vec2& size, const float rotation)
  : m_pTexture(std::move(pTexture)), m_pShaderProgram(std::move(pShaderProgram)), m_Position(position), m_Size(size),
    m_Rotation(rotation)
{
  const GLfloat verticiesCoords[] = {0.f, 0.f, 0.f, 1.f, 1.f, 1.f,

                                     1.f, 1.f, 1.f, 0.f, 0.f, 0.f};

  const GLfloat textureCoords[] = {0.f, 0.f, 0.f, 1.f, 1.f, 1.f,

                                   1.f, 1.f, 1.f, 0.f, 0.f, 0.f};

  glGenVertexArrays(1, &m_Vao);
  glBindVertexArray(m_Vao);

  glGenBuffers(1, &m_VertexCoordsVbo);
  glBindBuffer(GL_ARRAY_BUFFER, m_VertexCoordsVbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(verticiesCoords), &verticiesCoords, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

  glGenBuffers(1, &m_TextureCoordsVbo);
  glBindBuffer(GL_ARRAY_BUFFER, m_TextureCoordsVbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(textureCoords), &textureCoords, GL_STATIC_DRAW);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

Sprite::~Sprite()
{
  glDeleteBuffers(1, &m_VertexCoordsVbo);
  glDeleteBuffers(1, &m_TextureCoordsVbo);
  glDeleteBuffers(1, &m_Vao);
}

void Sprite::Render() const
{
  m_pShaderProgram->use();

  glm::mat4 model(1.f);
  model = glm::translate(model, glm::vec3(m_Position, 0.f));
  model = glm::rotate(model, glm::radians(m_Rotation), glm::vec3(0.f, 0.f, 1.f));
  model = glm::translate(model, glm::vec3(0.5f * m_Size.x, 0.5 * m_Size.y, 0.f));
  model = glm::scale(model, glm::vec3(m_Size, 1.f));

  glBindVertexArray(m_Vao);
  m_pShaderProgram->SetMatrix4("modelMatrix", model);
  glActiveTexture(GL_TEXTURE0);
  m_pTexture->Bind();
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glBindVertexArray(0);
}

void Sprite::SetPosition(const glm::vec2& position)
{
  m_Position = position;
}

void Sprite::SetSize(const glm::vec2& size)
{
  m_Size = size;
}

void Sprite::SetRotation(const float rotation)
{
  m_Rotation = rotation;
}
} // namespace Renderer
