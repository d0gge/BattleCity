#include "ResourceManager.h"
#include "../Renderer/ShaderProgram.h"
#include "../Renderer/Sprite.h"
#include "../Renderer/Texture2D.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdint.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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

std::shared_ptr<Renderer::ShaderProgram> ResourceManager::LoadShaders(const std::string& shaderName,
                                                                      const std::string& vertexPath,
                                                                      const std::string& fragmentPath)
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

  std::shared_ptr<Renderer::ShaderProgram>& newShader =
    m_ShaderProgramsMap.emplace(shaderName, std::make_shared<Renderer::ShaderProgram>(vertexString, fragmentString))
      .first->second;

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
  ShaderProgramsMap::const_iterator it = m_ShaderProgramsMap.find(shaderName);
  if (it != m_ShaderProgramsMap.end())
  {
    return it->second;
  }
  std::cerr << "Can't find a shader: " << shaderName << "\n";
  return nullptr;
}

std::shared_ptr<Renderer::Texture2D> ResourceManager::LoadTexture(const std::string& textureName,
                                                                  const std::string& texturePath)
{
  int channel = 0;
  int width = 0;
  int height = 0;
  stbi_set_flip_vertically_on_load(true);
  unsigned char* pixels = stbi_load(std::string(m_Path + "/" + texturePath).c_str(), &width, &height, &channel, 0);

  if (!pixels)
  {
    std::cerr << "Can't load image: " << texturePath << std::endl;
    return nullptr;
  }

  std::shared_ptr<Renderer::Texture2D> newTexture =
    m_TexturesMap
      .emplace(textureName,
               std::make_shared<Renderer::Texture2D>(width, height, pixels, channel, GL_NEAREST, GL_CLAMP_TO_EDGE))
      .first->second;

  stbi_image_free(pixels);
  return newTexture;
}

std::shared_ptr<Renderer::Texture2D> ResourceManager::GetTexture(const std::string& textureName) const
{
  TexturesMap::const_iterator it = m_TexturesMap.find(textureName);
  if (it != m_TexturesMap.end())
  {
    return it->second;
  }

  return nullptr;
}

std::shared_ptr<Renderer::Sprite> ResourceManager::LoadSprite(const std::string& spriteName,
                                                              const std::string& textureName,
                                                              const std::string& shaderName, const uint32_t spriteWidth,
                                                              const uint32_t spriteHeight,
                                                              const std::string& subTextureName)
{
  auto pTexture = GetTexture(textureName);
  if (!pTexture)
  {
    std::cerr << "Can't find a texture with name: " << textureName << " for a sprite: " << spriteName << std::endl;
  }

  auto pShader = GetShaderProgram(shaderName);
  if (!pShader)
  {
    std::cerr << "Can't find a shader with name: " << shaderName << "for a sprite: " << spriteName << std::endl;
  }

  auto newSprite =
    m_SpritesMap
      .emplace(shaderName, std::make_shared<Renderer::Sprite>(pTexture, subTextureName, pShader, glm::vec2(0.f, 0.f),
                                                              glm::vec2(spriteWidth, spriteHeight)))
      .first->second;
  return newSprite;
}

std::shared_ptr<Renderer::Sprite> ResourceManager::GetSprite(const std::string& spriteName) const
{
  SpritesMap::const_iterator it = m_SpritesMap.find(spriteName);
  if (it != m_SpritesMap.end())
  {
    return it->second;
  }
  return nullptr;
}

std::shared_ptr<Renderer::Texture2D> ResourceManager::LoadTextureAtlas(const std::string textureName,
                                                                       const std::string texturePath,
                                                                       std::vector<std::string> subTextures,
                                                                       const uint32_t subTextureWidth,
                                                                       const uint32_t subTextureHeight)
{
  auto pTexture = LoadTexture(std::move(textureName), std::move(texturePath));
  if (pTexture)
  {
    uint32_t textureWidth = pTexture->GetWidth();
    uint32_t textureHeight = pTexture->GetHeight();
    uint32_t currentTextureOffsetX = 0;
    uint32_t currentTextureOffsetY = textureHeight;
    for (const auto& currentSubTextureName : subTextures)
    {
      glm::vec2 leftBottomUV(static_cast<float>(currentTextureOffsetX) / textureWidth,
                             static_cast<float>(currentTextureOffsetY - subTextureHeight) / textureHeight);
      glm::vec2 rightTopUV(static_cast<float>(currentTextureOffsetX + subTextureWidth) / textureWidth,
                           static_cast<float>(currentTextureOffsetY) / textureHeight);
      pTexture->AddSubTexture(std::move(currentSubTextureName), leftBottomUV, rightTopUV);
      currentTextureOffsetX += subTextureWidth;
      if (currentTextureOffsetX >= textureWidth)
      {
        currentTextureOffsetX = 0;
        currentTextureOffsetY -= subTextureHeight;
      }
    }
  }
  return pTexture;
}
