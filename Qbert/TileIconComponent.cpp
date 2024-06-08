#include "TileIconComponent.h"
#include "LevelManagerComponent.h"
#include "SpritesheetComponent.h"

TileIconComponent::TileIconComponent(dae::GameObject* pGameObject, LevelManagerComponent* pLevelManagerComponent)
	: BaseComponent(pGameObject)
	, m_pLevelManagerComponent{ pLevelManagerComponent }
	, m_pGameResumedSubject{ pLevelManagerComponent->GameResumed.get() }
{
}

TileIconComponent::~TileIconComponent()
{
	if (m_pGameResumedSubject)
		m_pGameResumedSubject->RemoveObserver(this);
}

void TileIconComponent::Init()
{
	m_pGameResumedSubject->AddObserver(this);

	int level = m_pLevelManagerComponent->GetLevelNr();
	int round = m_pLevelManagerComponent->GetRoundNr();
	GetGameObject()->GetComponent<dae::SpritesheetComponent>()->MoveSourceRect(level + round - 2, (level == 2) ? 1 : 0);
}

void TileIconComponent::Notify(GameState gameState)
{
	if (gameState == GameState::QbertDied)
		return;

	int level = m_pLevelManagerComponent->GetLevelNr();
	int round = m_pLevelManagerComponent->GetRoundNr();
	GetGameObject()->GetComponent<dae::SpritesheetComponent>()->MoveSourceRect(level + round - 2, (level == 2) ? 1 : 0);
}

void TileIconComponent::SubjectDestroyed(dae::Subject<GameState>* pSubject)
{
	if (pSubject == m_pGameResumedSubject)
		m_pGameResumedSubject = nullptr;
}
