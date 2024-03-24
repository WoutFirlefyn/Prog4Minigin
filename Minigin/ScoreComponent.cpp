//---------------------------
// Includes
//---------------------------
#include "ScoreComponent.h"
#include "TextComponent.h"
#include "QbertComponent.h"
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
	if (m_pQbertComponent)
		m_pQbertComponent->ScoreChanged->RemoveObserver(this);
}

//---------------------------
// Member functions
//---------------------------

void dae::ScoreComponent::Init()
{
	m_pQbertComponent->ScoreChanged->AddObserver(this);
	m_pTextComponent = GetGameObject()->GetComponent<TextComponent>();
	UpdateText();
}

void dae::ScoreComponent::Notify()
{
	UpdateText();
}

void dae::ScoreComponent::SubjectDestroyed()
{
	m_pQbertComponent = nullptr;
}

void dae::ScoreComponent::UpdateText()
{
	if (m_pTextComponent)
		m_pTextComponent->SetText("Score: " + std::to_string(m_pQbertComponent->GetScore()));
}
