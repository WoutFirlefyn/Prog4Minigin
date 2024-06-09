#include <SDL.h>
#include "Texture2D.h"

dae::Texture2D::Texture2D(SDL_Texture* texture)
	: m_pTexture{ std::unique_ptr<SDL_Texture, DeleteTexture>(texture, DeleteTexture()) }
{
}

glm::ivec2 dae::Texture2D::GetSize() const
{
	SDL_Rect dst{};
	SDL_QueryTexture(GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
	return { dst.w,dst.h };
}

void dae::Texture2D::DeleteTexture::operator()(SDL_Texture* pTexture)
{
	SDL_DestroyTexture(pTexture);
}
