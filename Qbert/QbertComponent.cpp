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
	if (m_pCharacterFellSubject)
		m_pCharacterFellSubject->RemoveObserver(this);
}

void QbertComponent::Init()
{
	CharacterComponent::Init();
	m_pTileChangedSubject->AddObserver(this);
	m_pCharacterFellSubject->AddObserver(this);
	GetGameObject()->GetComponent<dae::SpritesheetComponent>()->MoveSourceRect(static_cast<int>(MovementDirection::Right), 0);
}

void QbertComponent::AddObserver(dae::Subject<bool>* pTileChangedSubject, dae::Subject<Character>* pCharacterFellSubject)
{
	m_pTileChangedSubject = pTileChangedSubject;
	m_pCharacterFellSubject = pCharacterFellSubject;
}

void QbertComponent::Notify(Character, MovementState movementState, MovementDirection movementDirection)
{
	switch (movementState)
	{
	case MovementState::Start:
		m_MovementDirection = movementDirection;
		GetGameObject()->GetComponent<dae::SpritesheetComponent>()->MoveSourceRect(static_cast<int>(m_MovementDirection), 0);
		m_AccumSec = 0.f;
		m_StartPos = GetGameObject()->GetLocalPosition();
		break;
	case MovementState::End:
		dae::ServiceLocator::GetSoundSystem().Play(dae::Sounds::QbertJump, 0.2f);
		m_MovementDirection = MovementDirection::None;
		break;
	default:
		break;
	}
}

void QbertComponent::Notify(Character character)
{
	if (character != Character::Qbert1)
		return;

	Die();
	GetGameObject()->SetPosition(m_StartPos);
}

void QbertComponent::SubjectDestroyed(dae::Subject<Character>* pSubject)
{
	if (pSubject == m_pCharacterFellSubject)
		m_pCharacterFellSubject = nullptr;
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

void QbertComponent::Die()
{
	PlayerDied->NotifyObservers(--m_Lives);
}
