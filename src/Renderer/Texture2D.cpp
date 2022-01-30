#include "Texture2D.h"
#include <memory>

namespace Renderer
{
  Texture2D::Texture2D(const GLuint width, GLuint height,
            const unsigned char* data, const unsigned int channels, 
            const GLenum filter, const GLenum wrapMode) 
            :
            m_Width(width),
            m_Height(height)
  {

    switch (channels)
    {
      case 4:
        m_Mode = GL_RGBA;
        break;
      case 3:
        m_Mode = GL_RGB;
        break;
      default:
        m_Mode = GL_RGBA;
        break;
    }
    glGenTextures(1, &m_Id);
    glBindTexture(GL_TEXTURE_2D, m_Id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, m_Mode, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
  }

  Texture2D& Texture2D::operator=(Texture2D&& texture2D)
  {
    glDeleteTextures(1, &m_Id);
    m_Id = texture2D.m_Id;
    texture2D.m_Id = 0;
    m_Mode = texture2D.m_Mode;
    m_Width = texture2D.m_Width;
    m_Height = texture2D.m_Height; 
    return *this;
  }

  Texture2D::Texture2D(Texture2D&& texture2D)
  {
    m_Id = texture2D.m_Id;
    texture2D.m_Id = 0;
    m_Mode = texture2D.m_Mode;
    m_Width = texture2D.m_Width;
    m_Height = texture2D.m_Height; 
  }

  Texture2D::~Texture2D()
  {
    glDeleteTextures(1, &m_Id);
  }


  void Texture2D::Bind() const
  {
    glBindTexture(GL_TEXTURE_2D, m_Id);
  }
}
