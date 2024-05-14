//---------------------------
// Includes
//---------------------------
#include "GraphicsComponent.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "GameObject.h"
#include "Texture2D.h"
#include "ServiceLocator.h"

dae::GraphicsComponent::GraphicsComponent(GameObject* pGameObject, const std::string& filename, bool renderingEnabled) : BaseComponent(pGameObject)
	, m_RenderingEnabled{ renderingEnabled }
{
	SetTexture(filename);
}

dae::GraphicsComponent::GraphicsComponent(GameObject* pGameObject) : BaseComponent(pGameObject)
{
}

void dae::GraphicsComponent::Render() const
{
	if (m_pTexture != nullptr && m_RenderingEnabled)
	{
		const auto& pos = GetGameObject()->GetWorldTransform().GetPosition();
		if (m_UseSourceRect)
			Renderer::GetInstance().RenderTexture(*m_pTexture, pos.x, pos.y, m_SrcRect);
		else
			Renderer::GetInstance().RenderTexture(*m_pTexture, pos.x, pos.y);
	}
}

void dae::GraphicsComponent::SetTexture(const std::string& filename)
{
	m_pTexture = ServiceLocator::GetResourceManager().LoadTexture(filename);
}

void dae::GraphicsComponent::SetTexture(Texture2D* pTexture)
{
	m_pTexture = pTexture;
}




