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
dae::GraphicsComponent::GraphicsComponent(GameObject* pGameObject, const std::string& filename) : BaseComponent(pGameObject)
{
	SetTexture(filename);
}

dae::GraphicsComponent::~GraphicsComponent()
{
	// nothing to destroy
}

void dae::GraphicsComponent::Render() const
{
	const auto& pos = m_pGameObject->GetPosition().GetPosition();
	Renderer::GetInstance().RenderTexture(*m_pTexture, pos.x, pos.y);
}

void dae::GraphicsComponent::Update(float deltaTime)
{
	(void)deltaTime;
}

void dae::GraphicsComponent::SetTexture(const std::string& filename)
{
	m_pTexture = ResourceManager::GetInstance().LoadTexture(filename);
}

//---------------------------
// Member functions
//---------------------------




