#pragma once
#include <glad/glad.h>
#include <string>

namespace Renderer
{
class Texture2D
{
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

private:
  GLuint m_Id;
  GLenum m_Mode;
  unsigned int m_Width;
  unsigned int m_Height;
};
} // namespace Renderer