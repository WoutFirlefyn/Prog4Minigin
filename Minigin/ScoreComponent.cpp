//---------------------------
// Includes
//---------------------------
#include "ScoreComponent.h"
#include "TextComponent.h"
#include "QbertComponent.h"
#include "TileComponent.h"
#include "LevelManagerComponent.h"
#include "GameObject.h"

//---------------------------
// Constructor & Destructor
//---------------------------
dae::ScoreComponent::ScoreComponent(GameObject* pGameObject, QbertComponent* pQbertComponent, LevelManagerComponent* pLevelManagerComponent)
	: BaseComponent(pGameObject)
	, m_pQbertComponent{ pQbertComponent }
	, m_pLevelManagerComponent{ pLevelManagerComponent }
{
}

dae::ScoreComponent::~ScoreComponent()
{
	if(m_pLevelManagerComponent)
		m_pLevelManagerComponent->TileChanged->RemoveObserver(this);
}

//---------------------------
// Member functions
//---------------------------

void dae::ScoreComponent::Init()
{
	m_pLevelManagerComponent->TileChanged->AddObserver(this);
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

void dae::ScoreComponent::SubjectDestroyed(Subject<bool>* pSubject)
{
	if (pSubject == m_pLevelManagerComponent->TileChanged.get())
		m_pLevelManagerComponent = nullptr;
}

void dae::ScoreComponent::UpdateText()
{
	if (m_pTextComponent)
		m_pTextComponent->SetText("Score: " + std::to_string(m_Score));
}
