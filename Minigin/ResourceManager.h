#pragma once
#include <string>
#include <memory>
#include <unordered_map>
#include <glm/glm.hpp>
#include "Singleton.h"
#include "Texture2D.h"

struct SDL_Color;
namespace dae
{
	class Font;
	class ResourceManager final : public Singleton<ResourceManager>
	{
	public:
		void Init(const std::string& data);
		Texture2D* LoadTexture(const std::string& file);
		std::unique_ptr<Texture2D> LoadTexture(Font* pFont, const glm::vec4& color, const std::string& text);
		std::shared_ptr<Font> LoadFont(const std::string& file, unsigned int size) const;
	private:
		friend class Singleton<ResourceManager>;
		ResourceManager() = default;
		std::string m_dataPath;

		std::unordered_map<std::string, std::unique_ptr<Texture2D>> m_mTextures;
	};
}
