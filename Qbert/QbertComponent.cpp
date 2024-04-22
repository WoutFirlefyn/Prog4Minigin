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
#include "SDLSoundSystem.h"

//---------------------------
// Constructor & Destructor
//---------------------------
QbertComponent::QbertComponent(dae::GameObject* pGameObject)
	: BaseComponent(pGameObject)
{
	PlayerDied = std::make_unique<dae::Subject<int>>();
	MoveStateChanged = std::make_unique<dae::Subject<Character, MovementState, MovementDirection>>();
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
	MoveStateChanged->AddObserver(this);
	m_pTileChangedSubject->AddObserver(this);
	m_pCharacterFellSubject->AddObserver(this);
	GetGameObject()->GetComponent<dae::SpritesheetComponent>()->MoveSourceRect(static_cast<int>(MovementDirection::Right), 0);
}

void QbertComponent::Update()
{
	if (m_MovementDirection == MovementDirection::None)
		return;

	glm::vec3 endPos{ m_StartPos };
	switch (m_MovementDirection)
	{
	case MovementDirection::Up:
		endPos += glm::vec3{ 16, -24, 0 };
		break;
	case MovementDirection::Down:
		endPos += glm::vec3{ -16, 24, 0 };
		break;
	case MovementDirection::Left:
		endPos += glm::vec3{ -16, -24, 0 };
		break;
	case MovementDirection::Right:
		endPos += glm::vec3{ 16, 24, 0 };
		break;
	default:
		return;
	}

	glm::vec3 control { m_StartPos };

	if (static_cast<int>(m_MovementDirection) < 2)
		control += (endPos - m_StartPos) * glm::vec3( 0.1f, 1.f, 0.f );
	else
		control += (endPos - m_StartPos) * glm::vec3{ 1.f, 0.1f, 0.f };

	m_AccumSec += dae::GameTime::GetInstance().GetDeltaTime();
	float t = std::min(m_AccumSec / m_JumpDuration, 1.f);

	glm::vec3 currentPos = (1 - t) * (1 - t) * m_StartPos + 2 * (1 - t) * t * control + t * t * endPos;

	GetGameObject()->SetPosition(currentPos);

	if (t >= 1.f)
	{
		MoveStateChanged->NotifyObservers(Character::Qbert1, MovementState::End, m_MovementDirection);
		m_MovementDirection = MovementDirection::None;
	}
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
		dae::ServiceLocator::GetSoundSystem().Play(static_cast<dae::SoundId>(Sounds::QbertJump));
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
