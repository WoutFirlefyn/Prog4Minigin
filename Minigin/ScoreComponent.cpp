//---------------------------
// Includes
//---------------------------
#include "ScoreComponent.h"
#include "TextComponent.h"
#include "QbertComponent.h"
#include "TileComponent.h"
#include "GameObject.h"

//---------------------------
// Constructor & Destructor
//---------------------------
dae::ScoreComponent::ScoreComponent(GameObject* pGameObject, QbertComponent* pQbertComponent)
	: BaseComponent(pGameObject)
	, m_pQbertComponent{ pQbertComponent }
{
}

dae::ScoreComponent::~ScoreComponent()
{
	dae::TileComponent::TileChanged->RemoveObserver(this);
}

//---------------------------
// Member functions
//---------------------------

void dae::ScoreComponent::Init()
{
	m_pTextComponent = GetGameObject()->GetComponent<TextComponent>();
	UpdateText();
}

void dae::ScoreComponent::Notify(bool roundFinished)
{
	m_Score += 25;
	if (roundFinished)
		m_Score += 100;
	UpdateText();
}

void dae::ScoreComponent::UpdateText()
{
	if (m_pTextComponent)
		m_pTextComponent->SetText("Score: " + std::to_string(m_Score));
}
