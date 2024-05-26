#include "QbertStates.h"
#include "QbertComponent.h"
#include "DiskComponent.h"
#include "GameTime.h"
#include "GameObject.h"
#include "LevelManagerComponent.h"
#include "SpritesheetComponent.h"
#include "ServiceLocator.h"
#include "Sounds.h"

void QbertIdleState::HandleInput(MovementInfo movementInfo)
{
	return SetState(std::make_unique<QbertJumpState>(m_pCharacter, movementInfo));
}

void QbertIdleState::Notify(Character character, Character otherCharacter)
{
	Character qbertCharacter = m_pCharacter->GetCharacter();

	if (character != qbertCharacter && otherCharacter != qbertCharacter)
		return;

	if (otherCharacter == qbertCharacter)
		std::swap(character, otherCharacter);

	switch (otherCharacter)
	{
	case Character::Coily:
	case Character::Ugg:
	case Character::Wrongway:
	{
		MovementInfo movementInfo{};
		movementInfo.state = MovementState::Fall;
		m_pCharacter->MoveStateChanged->NotifyObservers(qbertCharacter, movementInfo);
		return SetState(std::make_unique<QbertDeathState>(m_pCharacter));
	}
	default:
		return;
	}
}

void QbertJumpState::Update()
{
	if (Jump())
	{
		switch (m_NextTileType)
		{
		case TileType::Tile:
			return SetState(std::make_unique<QbertIdleState>(m_pCharacter));
		case TileType::Disk:
			return SetState(std::make_unique<QbertDiskState>(m_pCharacter));
		case TileType::None:
			return SetState(std::make_unique<QbertDeathState>(m_pCharacter, m_StartPos));
		}
	}
}

void QbertJumpState::OnEnter()
{
	JumpState::OnEnter();
	GetGameObject()->GetComponent<dae::SpritesheetComponent>()->MoveSourceRect(static_cast<int>(m_MovementInfo.direction), 0);
	m_JumpDuration = 0.1f;
}

void QbertJumpState::OnExit()
{
	switch (m_NextTileType)
	{
	case TileType::Tile:
		m_MovementInfo.state = MovementState::End;
		dae::ServiceLocator::GetSoundSystem().Play(dae::Sounds::QbertJump, 0.2f);
		break;
	case TileType::Disk:
		m_MovementInfo.state = MovementState::Disk;
		break;
	case TileType::None:
		m_MovementInfo.state = MovementState::Fall;
		dae::ServiceLocator::GetSoundSystem().Play(dae::Sounds::QbertFall, 0.2f);
		break;
	default:
		return;
	}

	m_pCharacter->MoveStateChanged->NotifyObservers(m_pCharacter->GetCharacter(), m_MovementInfo);
}

void QbertDeathState::Update()
{
	m_AccumSec += dae::GameTime::GetInstance().GetDeltaTime();

	if (m_AccumSec >= m_RespawnDelay)
		return SetState(std::make_unique<QbertIdleState>(m_pCharacter));
}

void QbertDeathState::OnExit()
{
	if (m_StartPos != glm::vec3(0))
		GetGameObject()->SetPosition(m_StartPos);
}

QbertDiskState::QbertDiskState(CharacterComponent* pCharacter)
	: CharacterState(pCharacter)
	, m_pDiskStateChangedSubject{ DiskComponent::DiskStateChanged.get() }

{
	if (m_pDiskStateChangedSubject)
		m_pDiskStateChangedSubject->AddObserver(this);
}

QbertDiskState::~QbertDiskState()
{
	if (m_pDiskStateChangedSubject)
		m_pDiskStateChangedSubject->RemoveObserver(this);
}

void QbertDiskState::Update()
{
	if (!m_DiskReachedTop)
		return;

	m_FallLerpValue = std::min(m_FallLerpValue + dae::GameTime::GetInstance().GetDeltaTime() / m_FallDuration, 1.f);
	GetGameObject()->SetPosition(m_StartPos + (m_TargetPos - m_StartPos) * m_FallLerpValue);

	if (m_FallLerpValue >= 1.f)
		return SetState(std::make_unique<QbertIdleState>(m_pCharacter));
}

void QbertDiskState::OnExit()
{
	MovementInfo movementInfo{};
	movementInfo.state = MovementState::End;
	m_pCharacter->MoveStateChanged->NotifyObservers(m_pCharacter->GetCharacter(), movementInfo);
}

void QbertDiskState::Notify(Disk disk, Character character)
{
	if (m_pCharacter->GetCharacter() != character)
		return;

	switch (disk.state)
	{
	case DiskState::Start:
		GetGameObject()->SetParent(disk.pGameObject, true);
		break;
	case DiskState::Stop:
		GetGameObject()->SetParent(nullptr);
		m_DiskReachedTop = true;
		m_StartPos = GetGameObject()->GetLocalPosition();
		m_TargetPos = m_StartPos + glm::vec3{ 0,64,0 };
		break;
	}

}

void QbertDiskState::SubjectDestroyed(dae::Subject<Disk, Character>* pSubject)
{
	if (m_pDiskStateChangedSubject == pSubject)
		m_pDiskStateChangedSubject = nullptr;
}

void QbertSpawnState::Update()
{
	return SetState(std::make_unique<QbertIdleState>(m_pCharacter));
}

void QbertSpawnState::OnEnter()
{
	SpawnState::OnEnter();

	GetGameObject()->GetComponent<dae::SpritesheetComponent>()->MoveSourceRect(static_cast<int>(MovementDirection::Right), 0);
}

void QbertResetState::Update()
{
	if (Wait())
		return SetState(std::make_unique<QbertSpawnState>(m_pCharacter));
}
