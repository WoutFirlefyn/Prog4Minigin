#include "QbertStates.h"
#include "QbertComponent.h"
#include "DiskComponent.h"
#include "GameTime.h"
#include "GameObject.h"
#include "LevelManagerComponent.h"
#include "SpritesheetComponent.h"
#include <iostream>

void QbertIdleState::HandleInput(MovementInfo movementInfo)
{
	return SetState(std::make_unique<QbertJumpState>(m_pCharacter, movementInfo));
}

void QbertIdleState::Notify(Character character, Character otherCharacter)
{
	if (character != GetCharacter() && otherCharacter != GetCharacter())
		return;

	if (otherCharacter == GetCharacter())
		std::swap(character, otherCharacter);

	switch (otherCharacter)
	{
	case Character::Coily:
	case Character::Ugg:
	case Character::Wrongway:
	{
		MovementInfo movementInfo{};
		movementInfo.state = MovementState::Fall;
		m_pCharacter->MoveStateChanged->NotifyObservers(GetCharacter(), movementInfo);
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
}

void QbertDeathState::Update()
{
	m_AccumSec += dae::GameTime::GetInstance().GetDeltaTime();

	if (m_AccumSec >= m_RespawnDelay)
		return SetState(std::make_unique<QbertIdleState>(m_pCharacter));
}

void QbertDeathState::OnExit()
{
	if (m_StartPos != glm::vec3{})
		GetGameObject()->SetPosition(m_StartPos);
}

QbertDiskState::~QbertDiskState()
{
	if (m_pDiskReachedTopSubject)
		m_pDiskReachedTopSubject->RemoveObserver(this);
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

void QbertDiskState::OnEnter()
{
	m_pDiskReachedTopSubject = DiskComponent::DiskReachedTop.get();
	m_pDiskReachedTopSubject->AddObserver(this);
}

void QbertDiskState::OnExit()
{
	MovementInfo movementInfo{};
	movementInfo.state = MovementState::End;
	m_pCharacter->MoveStateChanged->NotifyObservers(GetCharacter(), movementInfo);
}

void QbertDiskState::Notify(dae::GameObject*, Character character)
{
	if (GetCharacter() != character)
		return;

	GetGameObject()->SetParent(nullptr);
	m_DiskReachedTop = true;
	m_StartPos = GetGameObject()->GetLocalPosition();

	m_TargetPos = GetGameObject()->GetLocalPosition() + glm::vec3{ 0,64,0 };
}

void QbertDiskState::SubjectDestroyed(dae::Subject<dae::GameObject*, Character>* pSubject)
{
	if (m_pDiskReachedTopSubject == pSubject)
		m_pDiskReachedTopSubject = nullptr;
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
