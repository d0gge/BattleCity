#pragma once
#include <glad/glad.h>
#include <glm/vec2.hpp>
#include <string>
#include <map>

namespace Renderer
{
class Texture2D
{
public:
  struct SubTexture2D
  {
    glm::vec2 leftBottomUV;
    glm::vec2 rightTopUV;

    SubTexture2D(const glm::vec2& leftBottomUV_, const glm::vec2& rightTopUV_)
      : leftBottomUV(leftBottomUV_), rightTopUV(rightTopUV_)
    {
    }

    SubTexture2D() : leftBottomUV(0.f), rightTopUV(1.f)
    {
    }
  };

public:
  Texture2D(const GLuint width, GLuint height, const unsigned char* data, const unsigned int channels = 4,
            const GLenum filter = GL_LINEAR, const GLenum wrapMode = GL_CLAMP_TO_EDGE);

  Texture2D() = delete;
  Texture2D(const Texture2D&) = delete;
  Texture2D& operator=(const Texture2D&) = delete;
  Texture2D& operator=(Texture2D&& Texture2D);
  Texture2D(Texture2D&& Texture2D);
  ~Texture2D();

  void Bind() const;
  void AddSubTexture(const std::string subTextureName, const glm::vec2& leftBottomUV, const glm::vec2& rightTopUV);
  const SubTexture2D& GetSubTexture(const std::string& subTextureName) const;
  uint32_t GetWidth() const;
  uint32_t GetHeight() const;

private:
  GLuint m_Id;
  GLenum m_Mode;
  unsigned int m_Width;
  unsigned int m_Height;
  std::map<std::string, SubTexture2D> m_SubTexturesMap;
};
} // namespace Renderer