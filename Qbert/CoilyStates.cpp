#include "CoilyStates.h"
#include "CoilyComponent.h"
#include "GameTime.h"
#include "LevelManagerComponent.h"

void CoilyIdleState::Update()
{
	m_AccumSec += dae::GameTime::GetInstance().GetDeltaTime();

	if (m_AccumSec < m_TimeBetweenJumps)
		return;
	MovementInfo movementInfo{ MovementInfo::GetMovementInfo(static_cast<MovementDirection>(m_pCharacter->GetCharacter() == Character::Coily && CoilyComponent::IsEgg() ? 
		rand() % 2 + 2 : rand() % 4)) };
	return SetState(std::make_unique<CoilyJumpState>(m_pCharacter, movementInfo));
}

void CoilyJumpState::Update()
{
	if (Jump())
	{
		switch (m_NextTileType)
		{
		case TileType::Tile:
			return SetState(std::make_unique<CoilyIdleState>(m_pCharacter));
		case TileType::Disk:
		case TileType::None:
			return SetState(std::make_unique<CoilyDeathState>(m_pCharacter));
		}
	}
}

void CoilySpawnState::Update()
{
	if(Spawn())
		return SetState(std::make_unique<CoilyIdleState>(m_pCharacter));
}

void CoilySpawnState::OnEnter()
{ 
	SpawnState::OnEnter();
	m_TargetPos = GetPosition();
	SetPosition(m_TargetPos - glm::vec3{ 0.f, m_HeightOffset, 0.f });
}

void CoilyDeathState::Update()
{
	m_AccumSec += dae::GameTime::GetInstance().GetDeltaTime();

	if (m_AccumSec >= m_RespawnDelay)
		return SetState(std::make_unique<CoilySpawnState>(m_pCharacter));

	
}
