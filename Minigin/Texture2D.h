#pragma once
#include <glm/vec2.hpp>
#include <memory>

struct SDL_Rect;
struct SDL_Texture;
namespace dae
{
	/**
	 * Simple RAII wrapper for an SDL_Texture
	 */
	class Texture2D final
	{
	public:
		SDL_Texture* GetSDLTexture() const;
		explicit Texture2D(SDL_Texture* texture);
		~Texture2D();

		glm::ivec2 GetSize() const;

		void SetSourceRect(int x, int y, int width, int height);
		void SetSourceRect(int x, int y);
		const SDL_Rect* GetSourceRect() const;

		Texture2D(const Texture2D &) = delete;
		Texture2D(Texture2D &&) = delete;
		Texture2D & operator= (const Texture2D &) = delete;
		Texture2D & operator= (const Texture2D &&) = delete;
	private:
		SDL_Texture* m_texture;
		std::unique_ptr<SDL_Rect> m_pSrcRect;
	};
}
