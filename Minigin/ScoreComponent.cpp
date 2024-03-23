//---------------------------
// Includes
//---------------------------
#include "ScoreComponent.h"

//---------------------------
// Constructor & Destructor
//---------------------------
dae::ScoreComponent::ScoreComponent(GameObject* pGameObject)
	: BaseComponent(pGameObject)
{
	// nothing to create
}

//---------------------------
// Member functions
//---------------------------

void dae::ScoreComponent::Init()
{
}

void dae::ScoreComponent::Render() const
{
}

void dae::ScoreComponent::Update()
{
}

void dae::ScoreComponent::FixedUpdate()
{
}

void dae::ScoreComponent::RenderGUI()
{
}

void dae::ScoreComponent::IncreaseScore(int scoreIncrease)
{
	m_Score += scoreIncrease;
	NotifyObservers(GetGameObject());
}