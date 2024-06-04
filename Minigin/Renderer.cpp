#include <stdexcept>
#include <glm/glm.hpp>
#include "Renderer.h"
#include "SceneManager.h"
#include "Texture2D.h"
#include "Transform.h"
#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_opengl3.h"
#include <iostream>

int GetOpenGLDriverIndex()
{
	auto openglIndex = -1;
	const auto driverCount = SDL_GetNumRenderDrivers();
	for (auto i = 0; i < driverCount; i++)
	{
		SDL_RendererInfo info;
		if (!SDL_GetRenderDriverInfo(i, &info))
			if (!strcmp(info.name, "opengl"))
				openglIndex = i;
	}
	return openglIndex;
}

void dae::Renderer::Init(SDL_Window* window)
{
	m_window = window;
	m_renderer = SDL_CreateRenderer(window, GetOpenGLDriverIndex(), SDL_RENDERER_ACCELERATED);
	if (m_renderer == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
	}

	SDL_RenderSetVSync(m_renderer, 1);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplSDL2_InitForOpenGL(window, SDL_GL_GetCurrentContext());
	ImGui_ImplOpenGL3_Init();
}

void dae::Renderer::Render() const
{
	const auto& color = GetBackgroundColor();
	SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
	SDL_RenderClear(m_renderer);
	
	auto& sceneManager = SceneManager::GetInstance();
	sceneManager.Render();
	
	SDL_RenderPresent(m_renderer);
}

void dae::Renderer::Destroy()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	if (m_renderer != nullptr)
	{
		SDL_DestroyRenderer(m_renderer);
		m_renderer = nullptr;
	}
}

void dae::Renderer::RenderTexture(const Texture2D& texture, float x, float y, const glm::ivec4& srcRect) const
{
	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);

	if (srcRect == glm::ivec4{ 0 })
	{
		SDL_QueryTexture(texture.GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
		SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
		return;
	}

	dst.w = srcRect.z;
	dst.h = srcRect.w;

	SDL_Rect src{};
	src.x = srcRect.x;
	src.y = srcRect.y;
	src.w = srcRect.z;
	src.h = srcRect.w;

	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), &src, &dst);
}

void dae::Renderer::RenderTexture(const Texture2D& texture, const glm::vec4& dstRect, const glm::ivec4& srcRect) const
{
	SDL_Rect dst{};
	dst.x = static_cast<int>(dstRect.x);
	dst.y = static_cast<int>(dstRect.y);
	dst.w = static_cast<int>(dstRect.z);
	dst.h = static_cast<int>(dstRect.w);

	if (srcRect == glm::ivec4{ 0 })
	{
		SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
		return;
	}

	SDL_Rect src{};
	src.x = srcRect.x;
	src.y = srcRect.y;
	src.w = srcRect.z;
	src.h = srcRect.w;

	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), &src, &dst);
}

void dae::Renderer::RenderTexture(const Texture2D& texture, const Transform& transform, const glm::ivec4& srcRect) const
{
	const glm::vec3& pos = transform.GetPosition();
	const glm::vec3& scale = transform.GetScale();

	SDL_Rect dst{};
	dst.x = static_cast<int>(pos.x);
	dst.y = static_cast<int>(pos.y);

	if (srcRect == glm::ivec4{ 0 })
	{
		SDL_QueryTexture(texture.GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
		dst.w *= static_cast<int>(scale.x);
		dst.h *= static_cast<int>(scale.y);
		SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
		return;
	}

	dst.w = srcRect.z * static_cast<int>(scale.x);
	dst.h = srcRect.w * static_cast<int>(scale.y);
												
	SDL_Rect src{};
	src.x = srcRect.x;
	src.y = srcRect.y;
	src.w = srcRect.z;
	src.h = srcRect.w;

	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), &src, &dst);
}
