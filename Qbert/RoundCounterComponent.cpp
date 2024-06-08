#include "RoundCounterComponent.h"
#include "LevelManagerComponent.h"
#include "TextComponent.h"
#include <format>

RoundCounterComponent::RoundCounterComponent(dae::GameObject* pGameObject, LevelManagerComponent* pLevelManagerComponent)
	: BaseComponent(pGameObject)
	, m_pLevelManagerComponent{ pLevelManagerComponent }
	, m_pGameResumedSubject{ pLevelManagerComponent->GameResumed.get() }
{
	GetGameObject()->GetComponent<dae::TextComponent>()->SetText(std::format("Level:{}  Round:{}", m_pLevelManagerComponent->GetLevelNr(), m_pLevelManagerComponent->GetRoundNr()));
}

RoundCounterComponent::~RoundCounterComponent()
{
	if (m_pGameResumedSubject)
		m_pGameResumedSubject->RemoveObserver(this);
}

void RoundCounterComponent::Init()
{
	m_pGameResumedSubject->AddObserver(this);
}

void RoundCounterComponent::Notify(GameState gameState)
{
	if (gameState == GameState::QbertDied)
		return;

	GetGameObject()->GetComponent<dae::TextComponent>()->SetText(std::format("Level:{}  Round:{}", m_pLevelManagerComponent->GetLevelNr(), m_pLevelManagerComponent->GetRoundNr()));
}

void RoundCounterComponent::SubjectDestroyed(dae::Subject<GameState>* pSubject)
{
	if (pSubject == m_pGameResumedSubject)
		m_pGameResumedSubject = nullptr;
}
