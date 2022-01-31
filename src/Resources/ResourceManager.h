#pragma once
#include <string>
#include <memory>
#include <map>

namespace Renderer
{
  class ShaderProgram;
  class Texture2D;
}

class ResourceManager
{
public:
  ResourceManager(const std::string& executablePath);
  ~ResourceManager() = default;

  ResourceManager(const ResourceManager&) = delete;
  ResourceManager& operator=(const ResourceManager& ) = delete;
  ResourceManager& operator=(ResourceManager&&) = delete;
  ResourceManager(ResourceManager&&) = delete;

  std::shared_ptr<Renderer::ShaderProgram> LoadShaders(const std::string& shaderName, const std::string& vertexPath, const std::string& fragmentPath);
  std::shared_ptr<Renderer::ShaderProgram> GetShaderProgram(const std::string& shaderName);

  std::shared_ptr<Renderer::Texture2D> LoadTexture(const std::string& textureName, const std::string& texturePath);
  std::shared_ptr<Renderer::Texture2D> GetTexture(const std::string& textureName) const;


private:
  std::string GetFileString(const std::string& relativePath) const;

private:
  typedef std::map<const std::string, std::shared_ptr<Renderer::ShaderProgram>> ShaderProgramsMap;
  typedef std::map<const std::string, std::shared_ptr<Renderer::Texture2D>> TexturesMap;
  TexturesMap m_TexturesMap;
  ShaderProgramsMap m_ShaderProgramsMap;

  std::string m_Path; 
};