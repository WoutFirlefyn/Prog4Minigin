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
ScoreComponent::ScoreComponent(dae::GameObject* pGameObject, QbertComponent* pQbertComponent, LevelManagerComponent* pLevelManagerComponent)
	: BaseComponent(pGameObject)
	, m_pQbertComponent{ pQbertComponent }
	, m_pLevelManagerComponent{ pLevelManagerComponent }
{
}

ScoreComponent::~ScoreComponent()
{
	if(m_pLevelManagerComponent)
		m_pLevelManagerComponent->TileChanged->RemoveObserver(this);
}

//---------------------------
// Member functions
//---------------------------

void ScoreComponent::Init()
{
	m_pLevelManagerComponent->TileChanged->AddObserver(this);
	m_pTextComponent = GetGameObject()->GetComponent<dae::TextComponent>();
	UpdateText();
}

void ScoreComponent::Notify(bool roundFinished)
{
	m_Score += 25;
	if (roundFinished)
		m_Score += 100;
	UpdateText();
}

void ScoreComponent::SubjectDestroyed(dae::Subject<bool>* pSubject)
{
	if (pSubject == m_pLevelManagerComponent->TileChanged.get())
		m_pLevelManagerComponent = nullptr;
}

void ScoreComponent::UpdateText()
{
	if (m_pTextComponent)
		m_pTextComponent->SetText("Score: " + std::to_string(m_Score));
}
