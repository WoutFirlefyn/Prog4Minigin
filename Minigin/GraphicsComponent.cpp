//---------------------------
// Includes
//---------------------------
#include "GraphicsComponent.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "GameObject.h"

//---------------------------
// Constructor & Destructor
//---------------------------
dae::GraphicsComponent::GraphicsComponent()
{
	// nothing to create
}

dae::GraphicsComponent::~GraphicsComponent()
{
	// nothing to destroy
}

void dae::GraphicsComponent::Update(GameObject* pGameObject)
{
	const auto& pos = pGameObject->GetPosition().GetPosition();
	Renderer::GetInstance().RenderTexture(*m_pTexture, pos.x, pos.y);
}

void dae::GraphicsComponent::SetTexture(const std::string& filename)
{
	m_pTexture = ResourceManager::GetInstance().LoadTexture(filename);
}

//---------------------------
// Member functions
//---------------------------




