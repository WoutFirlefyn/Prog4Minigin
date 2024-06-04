#include <stdexcept>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL.h>
#include <iostream>
#include "ResourceManager.h"
#include "Renderer.h"
#include "Font.h"

dae::ResourceManager::ResourceManager()
{
	if (TTF_Init() != 0)
		throw std::runtime_error(std::string("Failed to load support for fonts: ") + SDL_GetError());
}

dae::Texture2D* dae::ResourceManager::LoadTexture(const std::string& file)
{
	if (!m_mTextures.contains(file))
	{
		const auto fullPath = m_DataPath + file;
		SDL_Texture* texture = IMG_LoadTexture(Renderer::GetInstance().GetSDLRenderer(), fullPath.c_str());
		if (texture == nullptr)
			throw std::runtime_error(std::string("Failed to load texture: ") + SDL_GetError());

		m_mTextures.emplace(file, std::make_unique<Texture2D>(texture));
	}

	return m_mTextures.at(file).get();
}

std::unique_ptr<dae::Texture2D> dae::ResourceManager::LoadTexture(Font* pFont, const glm::vec4& color, const std::string& text)
{
	SDL_Color sdlColor = 
	{ 
		static_cast<uint8_t>(color.r), 
		static_cast<uint8_t>(color.g), 
		static_cast<uint8_t>(color.b), 
		static_cast<uint8_t>(color.a) 
	};

	const auto surf = TTF_RenderText_Blended(pFont->GetFont(), text.c_str(), sdlColor);
	if (surf == nullptr)
		throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
	auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
	if (texture == nullptr)
		throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
	SDL_FreeSurface(surf);
	return std::make_unique<Texture2D>(texture);
}

std::shared_ptr<dae::Font> dae::ResourceManager::LoadFont(const std::string& file, unsigned int size) const
{
	return std::make_shared<Font>(m_DataPath + file, size);
}
