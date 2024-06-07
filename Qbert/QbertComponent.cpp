#include "QbertComponent.h"
#include "GameObject.h"
#include "SpritesheetComponent.h"
#include "TileComponent.h"
#include "ServiceLocator.h"
#include "Sounds.h"
#include "QbertStates.h"

QbertComponent::QbertComponent(dae::GameObject* pGameObject, LevelManagerComponent* pLevelManagerComponent, Character character)
	: CharacterComponent(pGameObject, pLevelManagerComponent)
{
	m_Character = character;
	PlayerDied = std::make_unique<dae::Subject<>>();
}

QbertComponent::~QbertComponent()
{
	MoveStateChanged->RemoveObserver(this);
}

void QbertComponent::Init()
{
	MoveStateChanged->AddObserver(this);

	SetState(std::make_unique<QbertResetState>(this));
}

void QbertComponent::LateUpdate()
{
	if (m_ResetCharacter)
	{
		m_ResetCharacter = false;
		SetState(std::make_unique<QbertResetState>(this), false);
	}
}

void QbertComponent::Notify(Character character, MovementInfo movementInfo)
{
	if (character == m_Character && movementInfo.state == MovementState::Fall)
	{
		--m_Lives;
		PlayerDied->NotifyObservers();
	}
}
