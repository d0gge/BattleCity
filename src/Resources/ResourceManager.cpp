#include "ResourceManager.h"
#include "../Renderer/ShaderProgram.h"
#include <sstream>
#include <fstream>
#include <iostream>

ResourceManager::ResourceManager(const std::string& executablePath)
{
  size_t lastDirectory = executablePath.find_last_of("/\\");
  m_Path = executablePath.substr(0, lastDirectory);
}

std::string ResourceManager::GetFileString(const std::string& relativePath) const
{
  std::ifstream f;
  f.open(m_Path + "/" + relativePath.c_str(), std::ios::in | std::ios::binary);
  if (!f.is_open())
  {
    std::cerr << "Failed to open file: " << relativePath << std::endl;
    return std::string{};
  }

  std::stringstream buffer;
  buffer << f.rdbuf();
  return buffer.str();
}

std::shared_ptr<Renderer::ShaderProgram> ResourceManager::LoadShaders(const std::string& shaderName,const std::string& vertexPath, const std::string& fragmentPath)
{
  std::string vertexString = GetFileString(vertexPath);
  if (vertexString.empty())
  {
    std::cerr << "No vertex shader" << std::endl;
    return nullptr;
  }


  std::string fragmentString = GetFileString(fragmentPath);
  if (fragmentString.empty())
  {
    std::cerr << "No fragment shader" << std::endl;
    return nullptr;
  }

 std::shared_ptr<Renderer::ShaderProgram>& newShader = m_ShaderPrograms.emplace(shaderName, std::make_shared<Renderer::ShaderProgram>(vertexString, fragmentString)).first->second;

 if (newShader->isCompiled())
 {
   return newShader;
 }

 std::cerr << "Can't load shader program: \n"
  << "vertex: " << vertexPath << "\n"
  << "Fragment: " << fragmentPath << "\n";
  std::cout << m_Path << std::endl;

  return nullptr;  
}

std::shared_ptr<Renderer::ShaderProgram> ResourceManager::GetShaderProgram(const std::string& shaderName) 
{
  ShaderProgramsMap::const_iterator it = m_ShaderPrograms.find(shaderName);
  if (it != m_ShaderPrograms.end())
  {
    return it->second;
  }
  std::cerr << "Can't find a shader: " << shaderName << "\n";
  return nullptr;
}