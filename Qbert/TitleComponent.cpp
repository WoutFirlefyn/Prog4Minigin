#include "TitleComponent.h"
#include "LevelManagerComponent.h"
#include "GraphicsComponent.h"
#include "SpritesheetComponent.h"
#include "Sounds.h"
#include "ServiceLocator.h"

TitleComponent::TitleComponent(dae::GameObject* pGameObject, LevelManagerComponent* pLevelManagerComponent)
	: BaseComponent(pGameObject)
	, m_pLevelManagerComponent{ pLevelManagerComponent }
	, m_pGameResumedSubject{ pLevelManagerComponent->GameResumed.get() }
{
}

void TitleComponent::Init()
{
	m_pGameResumedSubject->AddObserver(this);
	dae::ServiceLocator::GetSoundSystem().Play(dae::Sounds::LevelScreenTune);
}

void TitleComponent::Notify(GameState gameState)
{
	GetGameObject()->GetComponent<dae::GraphicsComponent>()->ToggleRendering(gameState == GameState::NextLevel);
	if (gameState == GameState::NextLevel)
	{
		dae::ServiceLocator::GetSoundSystem().Play(dae::Sounds::LevelScreenTune);
		GetGameObject()->GetComponent<dae::SpritesheetComponent>()->MoveSourceRectRelative(0, 1);
	}
}

void TitleComponent::SubjectDestroyed(dae::Subject<GameState>* pSubject)
{
	if (pSubject == m_pGameResumedSubject)
		m_pGameResumedSubject = nullptr;
}

