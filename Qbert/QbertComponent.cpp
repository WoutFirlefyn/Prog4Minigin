//---------------------------
// Includes
//---------------------------
#include "QbertComponent.h"
#include "GameObject.h"
#include "GameTime.h"
#include "SpritesheetComponent.h"
#include "TileComponent.h"
#include "LevelManagerComponent.h"
#include "ServiceLocator.h"
#include "Sounds.h"
#include "QbertStates.h"

//---------------------------
// Constructor & Destructor
//---------------------------
QbertComponent::QbertComponent(dae::GameObject* pGameObject)
	: CharacterComponent(pGameObject)
{
	PlayerDied = std::make_unique<dae::Subject<int>>();
}

QbertComponent::~QbertComponent()
{
	if (m_pTileChangedSubject)
		m_pTileChangedSubject->RemoveObserver(this);
}

void QbertComponent::Init()
{
	CharacterComponent::Init();

	m_pTileChangedSubject->AddObserver(this);

	m_Character = Character::Qbert1;
	SetState(std::make_unique<QbertSpawnState>(this));
}

void QbertComponent::AddObserver(dae::Subject<bool>* pTileChangedSubject, dae::Subject<Character, TileType>*)
{
	m_pTileChangedSubject = pTileChangedSubject;
}

void QbertComponent::Notify(Character character, MovementInfo movementInfo)
{
	if (character != m_Character)
		return;

	switch (movementInfo.state)
	{
	case MovementState::Start:
		GetGameObject()->GetComponent<dae::SpritesheetComponent>()->MoveSourceRect(static_cast<int>(movementInfo.direction), 0);
		break;
	case MovementState::End:
		dae::ServiceLocator::GetSoundSystem().Play(dae::Sounds::QbertJump, 0.2f);
		break;
	case MovementState::Fall:
		dae::ServiceLocator::GetSoundSystem().Play(dae::Sounds::QbertFall, 0.2f);
		PlayerDied->NotifyObservers(--m_Lives);
		break;
	case MovementState::Disk:
		dae::ServiceLocator::GetSoundSystem().Play(dae::Sounds::DiskLift, 0.2f);
		break;
	default:
		break;
	}
}

void QbertComponent::Notify(Character character)
{
	if (m_Character != character)
		return;

	CharacterComponent::Notify(character);

	GetGameObject()->GetComponent<dae::SpritesheetComponent>()->MoveSourceRect(static_cast<int>(MovementDirection::Right), 0);
}

void QbertComponent::Notify(bool roundFinished)
{
	if (roundFinished)
		GetGameObject()->SetPosition(308, 193);
}

void QbertComponent::SubjectDestroyed(dae::Subject<bool>* pSubject)
{
	if (pSubject == m_pTileChangedSubject)
		m_pTileChangedSubject = nullptr;
}
