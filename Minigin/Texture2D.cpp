#include <SDL.h>
#include "Texture2D.h"

dae::Texture2D::~Texture2D()
{
	SDL_DestroyTexture(m_texture);
}

glm::ivec2 dae::Texture2D::GetSize() const
{
	SDL_Rect dst;
	SDL_QueryTexture(GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
	return { dst.w,dst.h };
}

void dae::Texture2D::SetSourceRect(int x, int y, int width, int height)
{
	m_pSrcRect->x = x;
	m_pSrcRect->y = y;
	m_pSrcRect->w = width;
	m_pSrcRect->h = height;
}

void dae::Texture2D::SetSourceRect(int x, int y)
{
	SetSourceRect(x, y, m_pSrcRect->w, m_pSrcRect->h);
}

const SDL_Rect* dae::Texture2D::GetSourceRect() const
{
	return m_pSrcRect.get();
}

SDL_Texture* dae::Texture2D::GetSDLTexture() const
{
	return m_texture;
}

dae::Texture2D::Texture2D(SDL_Texture* texture)
{
	m_texture = texture;
	auto size = GetSize();
	m_pSrcRect = std::make_unique<SDL_Rect>();
	m_pSrcRect->w = size.x;
	m_pSrcRect->h = size.y;
}
