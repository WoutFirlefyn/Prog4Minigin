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
	if (filename != "")
		SetTexture(filename);
}

void dae::GraphicsComponent::Render() const
{
	if (m_pTexture != nullptr && m_RenderingEnabled)
	{
		if (m_UseSourceRect)
			Renderer::GetInstance().RenderTexture(*m_pTexture, GetGameObject()->GetWorldTransform(), m_SrcRect);
		else
			Renderer::GetInstance().RenderTexture(*m_pTexture, GetGameObject()->GetWorldTransform());
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

glm::ivec2 dae::GraphicsComponent::GetTextureSize() const
{
	glm::vec3 scale = GetGameObject()->GetWorldScale();

	if (m_UseSourceRect)
		return glm::ivec2(m_SrcRect.z * scale.x, m_SrcRect.w * scale.y);

	return m_pTexture->GetSize() * glm::ivec2{ scale.x, scale.y };
}




