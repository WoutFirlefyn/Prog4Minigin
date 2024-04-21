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
ScoreComponent::ScoreComponent(dae::GameObject* pGameObject)
	: BaseComponent(pGameObject)
{
}

ScoreComponent::~ScoreComponent()
{
	if (m_pTileChangedSubject)
		m_pTileChangedSubject->RemoveObserver(this);
}

//---------------------------
// Member functions
//---------------------------

void ScoreComponent::Init()
{
	m_pTileChangedSubject->AddObserver(this);
	m_pTextComponent = GetGameObject()->GetComponent<dae::TextComponent>();
	UpdateText();
}

void ScoreComponent::AddObserver(dae::Subject<bool>* pTileChangedSubject)
{
	m_pTileChangedSubject = pTileChangedSubject;
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
	if (pSubject == m_pTileChangedSubject)
		m_pTileChangedSubject = nullptr;
}

void ScoreComponent::UpdateText()
{
	if (m_pTextComponent)
		m_pTextComponent->SetText("Score: " + std::to_string(m_Score));
}
