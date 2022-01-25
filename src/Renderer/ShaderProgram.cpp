#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "ShaderProgram.h"
#include <iostream>

namespace Renderer
{

  bool ShaderProgram::createShader(const std::string& source, const GLenum shaderType, GLuint& shaderId)
  {
    shaderId = glCreateShader(shaderType); 
    const char* code = source.c_str();
    glShaderSource(shaderId, 1, &code, nullptr);
    glCompileShader(shaderId);
    
    GLint success;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
    if (!success)
    {
      GLchar infoLog[1024];
      glGetShaderInfoLog(shaderId, 1024, nullptr, infoLog);
      std::cerr << "ERROR::SHADER: Compile-time error:\n" << infoLog << std::endl;
      return false;
    }
    return true;
  }

  ShaderProgram::ShaderProgram(const std::string& vertexShader, const std::string& fragmentShader)
  {
    GLuint vertexShaderID;
    if (!createShader(vertexShader, GL_VERTEX_SHADER, vertexShaderID))
    {
      std::cerr << "VERTEX SHADER COMPILE-TIME ERROR" << std::endl;
      return;
    }

    GLuint fragmentShaderID;
    if (!createShader(fragmentShader, GL_FRAGMENT_SHADER, fragmentShaderID))
    {
      std::cerr << "FRAGMENT SHADER COMPLIE-TIME ERROR" << std::endl;
      glDeleteShader(vertexShaderID);
      return;
    }

    m_ID = glCreateProgram();
    glAttachShader(m_ID, vertexShaderID);
    glAttachShader(m_ID, fragmentShaderID);
    glLinkProgram(m_ID);

    GLint success; 
    glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
    if (!success)
    {
      GLchar infoLog[1024];
      glGetShaderInfoLog(m_ID, 1024, nullptr, infoLog);
      std::cerr << "ERROR::SHADER: LINK-TIME ERROR:\n" << infoLog << std::endl;
    }
    else
    {
      m_isCompiled = true;
    }

    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);
  } 


  ShaderProgram::~ShaderProgram()
  {
    glDeleteProgram(m_ID);
  }

  void ShaderProgram::use() const
  {
    glUseProgram(m_ID);
  }

  ShaderProgram& ShaderProgram::operator=(ShaderProgram&& shaderProgram) noexcept
  {
    glDeleteProgram(m_ID);
    m_ID = shaderProgram.m_ID;
    m_isCompiled = shaderProgram.m_isCompiled;

    shaderProgram.m_ID = 0;
    shaderProgram.m_isCompiled = false; 
    return *this;
  }

  ShaderProgram::ShaderProgram(ShaderProgram&& shaderProgram) noexcept
  {
    m_ID = shaderProgram.m_ID;
    m_isCompiled = shaderProgram.m_isCompiled;

    shaderProgram.m_ID = 0;
    shaderProgram.m_isCompiled = false;
  }
}