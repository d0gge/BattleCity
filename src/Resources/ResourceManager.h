#pragma once
#include <map>
#include <memory>
#include <string>

namespace Renderer
{
class ShaderProgram;
class Texture2D;
class Sprite;
} // namespace Renderer

class ResourceManager
{
public:
  ResourceManager(const std::string& executablePath);
  ~ResourceManager() = default;

  ResourceManager(const ResourceManager&) = delete;
  ResourceManager& operator=(const ResourceManager&) = delete;
  ResourceManager& operator=(ResourceManager&&) = delete;
  ResourceManager(ResourceManager&&) = delete;

  std::shared_ptr<Renderer::ShaderProgram> LoadShaders(const std::string& shaderName, const std::string& vertexPath,
                                                       const std::string& fragmentPath);
  std::shared_ptr<Renderer::ShaderProgram> GetShaderProgram(const std::string& shaderName);
  std::shared_ptr<Renderer::Texture2D> LoadTexture(const std::string& textureName, const std::string& texturePath);
  std::shared_ptr<Renderer::Texture2D> GetTexture(const std::string& textureName) const;
  std::shared_ptr<Renderer::Sprite> GetSprite(const std::string& spriteName) const;
  std::shared_ptr<Renderer::Sprite> LoadSprite(const std::string& spriteName, const std::string& textureName,
                                               const std::string& shaderName, const unsigned int spriteWidth,
                                               const unsigned int spriteHeight);

private:
  std::string GetFileString(const std::string& relativePath) const;

private:
  typedef std::map<const std::string, std::shared_ptr<Renderer::ShaderProgram>> ShaderProgramsMap;
  typedef std::map<const std::string, std::shared_ptr<Renderer::Texture2D>> TexturesMap;
  typedef std::map<const std::string, std::shared_ptr<Renderer::Sprite>> SpritesMap;
  TexturesMap m_TexturesMap;
  ShaderProgramsMap m_ShaderProgramsMap;
  SpritesMap m_SpritesMap;

  std::string m_Path;
};