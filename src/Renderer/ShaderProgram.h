#pragma once
#include <glad/glad.h>
#include <glm/mat4x4.hpp>
#include <string>

namespace Renderer
{

class ShaderProgram
{
public:
  ShaderProgram(const std::string& vertexShader, const std::string& fragmentShader);
  ~ShaderProgram();
  bool isCompiled() const
  {
    return m_isCompiled;
  }
  void use() const;
  void SetInt(const std::string& name, const GLint value);
  void SetMatrix4(const std::string& matrixName, const glm::mat4& matrix);

  ShaderProgram() = delete;
  ShaderProgram(ShaderProgram&) = delete;
  ShaderProgram& operator=(const ShaderProgram&) = delete;
  ShaderProgram& operator=(ShaderProgram&& shaderProgram) noexcept;
  ShaderProgram(ShaderProgram&& shaderProgram) noexcept;

private:
  bool createShader(const std::string& source, const GLenum shaderType, GLuint& shaderId);
  bool m_isCompiled = false;
  GLuint m_ID;
};
} // namespace Renderer