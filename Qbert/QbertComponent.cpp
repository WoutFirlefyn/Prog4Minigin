#include "QbertComponent.h"
#include "GameObject.h"
#include "SpritesheetComponent.h"
#include "TileComponent.h"
#include "ServiceLocator.h"
#include "Sounds.h"
#include "QbertStates.h"

QbertComponent::QbertComponent(dae::GameObject* pGameObject, LevelManagerComponent* pLevelManagerComponent)
	: CharacterComponent(pGameObject, pLevelManagerComponent)
{
	PlayerDied = std::make_unique<dae::Subject<>>();
}

QbertComponent::~QbertComponent()
{
	MoveStateChanged->RemoveObserver(this);
}

void QbertComponent::Init()
{
	m_vSpawnPositions.push_back({ 0,0 });

	MoveStateChanged->AddObserver(this);

	m_Character = Character::Qbert1;
	SetState(std::make_unique<QbertSpawnState>(this));
}

void QbertComponent::AddObserver(dae::Subject<bool>* /*pTileChangedSubject*/)
{
	//m_pTileChangedSubject = pTileChangedSubject;
}

void QbertComponent::Notify(Character character, MovementInfo movementInfo)
{
	if (character != m_Character)
		return;

	switch (movementInfo.state)
	{
	case MovementState::Fall:
		--m_Lives;
		PlayerDied->NotifyObservers();
		break;
	default:
		break;
	}
}

void QbertComponent::Notify(bool roundFinished)
{
	if (roundFinished)
		SetState(std::make_unique<QbertResetState>(this), false);
}
