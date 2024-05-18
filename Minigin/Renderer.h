#pragma once
#include <SDL.h>
#include <glm/glm.hpp>
#include "Singleton.h"

namespace dae
{
	class Texture2D;
	class Transform;
	class Renderer final : public Singleton<Renderer>
	{
		SDL_Renderer* m_renderer{};
		SDL_Window* m_window{};
		SDL_Color m_clearColor{};	
	public:
		void Init(SDL_Window* window);
		void Render() const;
		void Destroy();

		void RenderTexture(const Texture2D& texture, float x, float y, const glm::ivec4& srcRect = glm::ivec4(0)) const;
		void RenderTexture(const Texture2D& texture, const glm::vec4& dstRect, const glm::ivec4& srcRect = glm::ivec4(0)) const;
		void RenderTexture(const Texture2D& texture, const Transform& transform, const glm::ivec4& srcRect = glm::ivec4(0)) const;

		SDL_Renderer* GetSDLRenderer() const { return m_renderer; }

		const SDL_Color& GetBackgroundColor() const { return m_clearColor; }
		void SetBackgroundColor(const SDL_Color& color) { m_clearColor = color; }
	};
}

