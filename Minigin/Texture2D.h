#pragma once
#include <glm/vec2.hpp>
#include <memory>

struct SDL_Texture;
namespace dae
{
	class Texture2D final
	{
	public:
		explicit Texture2D(SDL_Texture* texture);
		~Texture2D() = default;

		SDL_Texture* GetSDLTexture() const { return m_pTexture.get(); };
		glm::ivec2 GetSize() const;

		Texture2D(const Texture2D &) = delete;
		Texture2D(Texture2D &&) = delete;
		Texture2D & operator= (const Texture2D &) = delete;
		Texture2D & operator= (const Texture2D &&) = delete;
	private:
		struct DeleteTexture
		{
			void operator()(SDL_Texture* pTexture);
		};
		std::unique_ptr<SDL_Texture, DeleteTexture> m_pTexture;
	};
}
