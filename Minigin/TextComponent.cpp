//---------------------------
// Includes
//---------------------------
#include <stdexcept>
#include "TextComponent.h"
#include "GraphicsComponent.h"
#include "GameObject.h"
#include "Font.h"
#include "Texture2D.h"
#include "ResourceManager.h"
#include "ServiceLocator.h"

dae::TextComponent::TextComponent(GameObject* pGameObject, std::shared_ptr<Font> pFont, const std::string& text) : BaseComponent(pGameObject)
	, m_Text{ text }
	, m_pFont{ pFont }
{
	if (text != "")
	{
		m_pTexture = ServiceLocator::GetResourceManager().LoadTexture(m_pFont.get(), m_Color, m_Text);
		GetGameObject()->GetComponent<GraphicsComponent>()->SetTexture(m_pTexture.get());
	}
}

void dae::TextComponent::Update()
{
	if (m_NeedsUpdate)
	{
		m_pTexture = ServiceLocator::GetResourceManager().LoadTexture(m_pFont.get(), m_Color, m_Text);
		GetGameObject()->GetComponent<GraphicsComponent>()->SetTexture(m_pTexture.get());
		m_NeedsUpdate = false;
	}
}

void dae::TextComponent::SetText(const std::string& text)
{
	if (m_Text == text)
		return;
	m_Text = text;
	m_NeedsUpdate = true;
}

void dae::TextComponent::SetColor(const glm::vec4& color)
{
	if (m_Color == color)
		return;
	m_Color = color;

	if (!m_Text.empty())
		m_NeedsUpdate = true;
}