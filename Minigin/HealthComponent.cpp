//---------------------------
// Includes
//---------------------------
#include "HealthComponent.h"

//---------------------------
// Constructor & Destructor
//---------------------------
dae::HealthComponent::HealthComponent(GameObject* pGameObject)
	: BaseComponent(pGameObject)
{
	// nothing to create
}

//---------------------------
// Member functions
//---------------------------

void dae::HealthComponent::Init()
{
}

void dae::HealthComponent::Render() const
{
}

void dae::HealthComponent::Update()
{
}

void dae::HealthComponent::FixedUpdate()
{
}

void dae::HealthComponent::RenderGUI()
{
}

void dae::HealthComponent::LoseALife()
{
	--m_Lives;
	NotifyObservers(GetGameObject());
}
