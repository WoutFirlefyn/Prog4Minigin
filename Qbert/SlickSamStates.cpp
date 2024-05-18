#include "SlickSamStates.h"
#include "CoilyComponent.h"
#include "GameTime.h"
#include "GameObject.h"
#include "LevelManagerComponent.h"

void SlickSamIdleState::Update()
{
	m_AccumSec += dae::GameTime::GetInstance().GetDeltaTime();

	if (m_AccumSec < m_TimeBetweenJumps)
		return;
	MovementInfo movementInfo{ MovementInfo::GetMovementInfo(static_cast<MovementDirection>(rand() % 2 + 2)) };
	return SetState(std::make_unique<SlickSamJumpState>(m_pCharacter, movementInfo));
}

void SlickSamIdleState::Notify(Character character, Character otherCharacter)
{
	if (character != GetCharacter() && otherCharacter != GetCharacter())
		return;

	if (otherCharacter == GetCharacter())
		std::swap(character, otherCharacter);

	switch (otherCharacter)
	{
	case Character::Qbert1:
	case Character::Qbert2:
	{
		MovementInfo movementInfo{};
		movementInfo.state = MovementState::Fall;
		m_pCharacter->MoveStateChanged->NotifyObservers(GetCharacter(), movementInfo);
		return SetState(std::make_unique<SlickSamDeathState>(m_pCharacter));
	}
	default:
		return;
	}
}

void SlickSamJumpState::Update()
{
	if (Jump())
	{
		switch (m_NextTileType)
		{
		case TileType::Tile:
			return SetState(std::make_unique<SlickSamIdleState>(m_pCharacter));
		case TileType::Disk:
		case TileType::None:
			return SetState(std::make_unique<SlickSamDeathState>(m_pCharacter));
		}
	}
}

void SlickSamSpawnState::Update()
{
	if (Spawn())
		return SetState(std::make_unique<SlickSamIdleState>(m_pCharacter));
}

void SlickSamSpawnState::OnEnter()
{ 
	SpawnState::OnEnter();
	m_TargetPos = GetGameObject()->GetLocalPosition();
	GetGameObject()->SetPosition(m_TargetPos - glm::vec3{ 0.f, m_HeightOffset, 0.f });
}

void SlickSamDeathState::Update()
{
	m_AccumSec += dae::GameTime::GetInstance().GetDeltaTime();

	if (m_AccumSec >= m_RespawnDelay)
		return SetState(std::make_unique<SlickSamSpawnState>(m_pCharacter));

	
}
