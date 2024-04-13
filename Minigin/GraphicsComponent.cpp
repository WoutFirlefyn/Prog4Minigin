//---------------------------
// Includes
//---------------------------
#include "GraphicsComponent.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "GameObject.h"
#include "Texture2D.h"

//---------------------------
// Constructor & Destructor
//---------------------------
dae::GraphicsComponent::GraphicsComponent(GameObject* pGameObject, const std::string& filename, bool renderingEnabled) : BaseComponent(pGameObject)
	, m_RenderingEnabled{ renderingEnabled }
{
	SetTexture(filename);
}

void dae::GraphicsComponent::Render() const
{
	if (m_pTexture != nullptr && m_RenderingEnabled)
	{
		const auto& pos = GetGameObject()->GetWorldTransform().GetPosition();
		Renderer::GetInstance().RenderTexture(*m_pTexture, pos.x, pos.y);
	}
}

void dae::GraphicsComponent::SetTexture(const std::string& filename)
{
	m_pTexture = ResourceManager::GetInstance().LoadTexture(filename);
}




