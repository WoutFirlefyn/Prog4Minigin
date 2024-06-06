#include "TitleComponent.h"
#include "LevelManagerComponent.h"
#include "GraphicsComponent.h"
#include "SpritesheetComponent.h"
#include "Sounds.h"
#include "ServiceLocator.h"

TitleComponent::TitleComponent(dae::GameObject* pGameObject, LevelManagerComponent* pLevelManagerComponent)
	: BaseComponent(pGameObject)
	, m_pLevelManagerComponent{ pLevelManagerComponent }
	, m_pNewRoundStartedSubject{ pLevelManagerComponent->NewRoundStarted.get() }
{
}

void TitleComponent::Init()
{
	m_pNewRoundStartedSubject->AddObserver(this);
	dae::ServiceLocator::GetSoundSystem().Play(dae::Sounds::LevelScreenTune);
}

void TitleComponent::Notify(bool nextLevel)
{
	GetGameObject()->GetComponent<dae::GraphicsComponent>()->ToggleRendering(nextLevel);
	if (nextLevel)
	{
		dae::ServiceLocator::GetSoundSystem().Play(dae::Sounds::LevelScreenTune);
		GetGameObject()->GetComponent<dae::SpritesheetComponent>()->MoveSourceRectRelative(0, 1);
	}
}

void TitleComponent::SubjectDestroyed(dae::Subject<bool>* pSubject)
{
	if (pSubject == m_pNewRoundStartedSubject)
		m_pNewRoundStartedSubject = nullptr;
}

