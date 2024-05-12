#include "QbertStates.h"
#include "QbertComponent.h"
#include "DiskComponent.h"
#include "GameTime.h"
#include "LevelManagerComponent.h"
#include <iostream>

void QbertIdleState::HandleInput(MovementDirection movementDirection)
{
	return m_pCharacter->SetState(std::make_unique<QbertJumpState>(m_pCharacter, movementDirection));
}

void QbertIdleState::Notify(Character character1, Character character2)
{
	if (character1 == m_pCharacter->GetCharacter() ||character2 == m_pCharacter->GetCharacter())
		return m_pCharacter->SetState(std::make_unique<QbertDeathState>(m_pCharacter));
}

void QbertJumpState::Update()
{
	if (Jump())
	{
		switch (m_pCharacter->GetNextTileType())
		{
		case TileType::Tile:
			return m_pCharacter->SetState(std::make_unique<QbertIdleState>(m_pCharacter));
		case TileType::Disk:
			return m_pCharacter->SetState(std::make_unique<QbertDiskState>(m_pCharacter));
		case TileType::None:
			return m_pCharacter->SetState(std::make_unique<QbertDeathState>(m_pCharacter, m_StartPos));
		}
	}
}

void QbertDeathState::Update()
{
	m_AccumSec += dae::GameTime::GetInstance().GetDeltaTime();

	if (m_AccumSec >= m_RespawnDelay)
		return m_pCharacter->SetState(std::make_unique<QbertIdleState>(m_pCharacter));
}

void QbertDeathState::OnExit()
{
	if (m_StartPos != glm::vec3{})
		m_pCharacter->SetPosition(m_StartPos);
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
		return m_pCharacter->SetState(std::make_unique<QbertIdleState>(m_pCharacter));
}

void QbertSpawnState::Update()
{
	return m_pCharacter->SetState(std::make_unique<QbertIdleState>(m_pCharacter));
}
