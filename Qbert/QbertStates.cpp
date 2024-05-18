#include "QbertStates.h"
#include "QbertComponent.h"
#include "DiskComponent.h"
#include "GameTime.h"
#include "LevelManagerComponent.h"
#include <iostream>

void QbertIdleState::HandleInput(MovementInfo movementInfo)
{
	return SetState(std::make_unique<QbertJumpState>(m_pCharacter, movementInfo));
}

void QbertIdleState::Notify(Character character, Character otherCharacter)
{
	if (character != m_pCharacter->GetCharacter() && otherCharacter != m_pCharacter->GetCharacter())
		return;

	if (otherCharacter == m_pCharacter->GetCharacter())
		std::swap(character, otherCharacter);

	switch (otherCharacter)
	{
	case Character::Coily:
	case Character::Ugg:
	case Character::Wrongway:
	{
		MovementInfo movementInfo{};
		movementInfo.state = MovementState::Fall;
		m_pCharacter->MoveStateChanged->NotifyObservers(m_pCharacter->GetCharacter(), movementInfo);
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

void QbertDeathState::Update()
{
	m_AccumSec += dae::GameTime::GetInstance().GetDeltaTime();

	if (m_AccumSec >= m_RespawnDelay)
		return SetState(std::make_unique<QbertIdleState>(m_pCharacter));
}

void QbertDeathState::OnExit()
{
	if (m_StartPos != glm::vec3{})
		SetPosition(m_StartPos);
}

void QbertDiskState::Update()
{
	m_AccumSec += dae::GameTime::GetInstance().GetDeltaTime();

	if (!m_HasReachedTop && m_AccumSec >= DiskComponent::m_TimeToReachTop)
	{
		m_AccumSec -= DiskComponent::m_TimeToReachTop;
		m_HasReachedTop = true;
	}

	// Todo: Falling after disk is at the end
	if (m_HasReachedTop)
		return SetState(std::make_unique<QbertIdleState>(m_pCharacter));
}

void QbertDiskState::OnExit()
{
	//m_pCharacter->MoveStateChanged->NotifyObservers(m_pCharacter->GetCharacter(), MovementState::Disk, MovementDirection::None);
}

void QbertSpawnState::Update()
{
	return SetState(std::make_unique<QbertIdleState>(m_pCharacter));
}
