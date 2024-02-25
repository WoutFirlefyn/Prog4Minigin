//---------------------------
// Includes
//---------------------------
#include <SDL_ttf.h>
#include <stdexcept>
#include "TextComponent.h"
#include "Renderer.h"
#include "GameObject.h"
#include "Font.h"
#include "Texture2D.h"
//---------------------------
// Constructor & Destructor
//---------------------------
dae::TextComponent::TextComponent(GameObject* pGameObject, const std::string& text, std::shared_ptr<Font> pFont) : BaseComponent(pGameObject)
	, m_NeedsUpdate{ true }
	, m_Text{ text }
	, m_pFont{ pFont }
	, m_pTextTexture{ nullptr }
{
	// nothing to create
}

dae::TextComponent::~TextComponent()
{
	// nothing to destroy
}

//---------------------------
// Member functions
//---------------------------

void dae::TextComponent::Render() const
{
	if (m_pTextTexture != nullptr)
	{
		const auto& pos = m_pGameObject->GetPosition().GetPosition();
		Renderer::GetInstance().RenderTexture(*m_pTextTexture, pos.x, pos.y);
	}
}

void dae::TextComponent::Update(float deltaTime)
{
	(void)deltaTime;
	if (m_NeedsUpdate)
	{
		const SDL_Color color = { 255,255,255,255 }; // only white text is supported now
		const auto surf = TTF_RenderText_Blended(m_pFont->GetFont(), m_Text.c_str(), color);
		if (surf == nullptr)
		{
			throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
		}
		auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
		if (texture == nullptr)
		{
			throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
		}
		SDL_FreeSurface(surf);
		m_pTextTexture = std::make_shared<Texture2D>(texture);
		m_NeedsUpdate = false;
	}
}

void dae::TextComponent::SetText(const std::string& text)
{
	m_Text = text;
	m_NeedsUpdate = true;
}




