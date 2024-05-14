#include "EnemyStates.h"
#include "CoilyComponent.h"
#include "GameTime.h"
#include "LevelManagerComponent.h"

void EnemyIdleState::Update()
{
	m_AccumSec += dae::GameTime::GetInstance().GetDeltaTime();

	if (m_AccumSec < m_TimeBetweenJumps)
		return;
	MovementInfo movementInfo{ MovementInfo::GetMovementInfo(static_cast<MovementDirection>((m_pCharacter->GetCharacter() == Character::Coily && CoilyComponent::IsEgg()) || (m_pCharacter->GetCharacter() == Character::Slick || m_pCharacter->GetCharacter() == Character::Sam) ? rand() % 2 + 2 : rand() % 4)) };
	return m_pCharacter->SetState(std::make_unique<EnemyJumpState>(m_pCharacter, movementInfo));
}

void EnemyIdleState::Notify(Character character, Character otherCharacter)
{
	if (character != m_pCharacter->GetCharacter() && otherCharacter != m_pCharacter->GetCharacter())
		return;

	if (otherCharacter == m_pCharacter->GetCharacter())
		std::swap(character, otherCharacter);

	switch (character)
	{
	case Character::Slick:
	case Character::Sam:
		break;
	default:
		return;
	}

	switch (otherCharacter)
	{
	case Character::Qbert1:
	case Character::Qbert2:
	{
		MovementInfo movementInfo{};
		movementInfo.state = MovementState::Fall;
		m_pCharacter->MoveStateChanged->NotifyObservers(m_pCharacter->GetCharacter(), movementInfo);
		return m_pCharacter->SetState(std::make_unique<EnemyDeathState>(m_pCharacter));
	}
	default:
		return;
	}
}

void EnemyJumpState::Update()
{
	if (Jump())
	{
		switch (m_NextTileType)
		{
		case TileType::Tile:
			return m_pCharacter->SetState(std::make_unique<EnemyIdleState>(m_pCharacter));
		case TileType::Disk:
		case TileType::None:
			return m_pCharacter->SetState(std::make_unique<EnemyDeathState>(m_pCharacter));
		}
	}
}

void EnemySpawnState::Update()
{
	m_FallLerpValue += dae::GameTime::GetInstance().GetDeltaTime() / m_FallDuration;
	m_FallLerpValue = std::min(m_FallLerpValue, 1.f);
	m_pCharacter->SetPosition(m_TargetPos -  glm::vec3{ 0.f, m_HeightOffset * (1.f - m_FallLerpValue), 0.f });

	if (m_FallLerpValue >= 1.f)
		return m_pCharacter->SetState(std::make_unique<EnemyIdleState>(m_pCharacter));
}

void EnemySpawnState::OnEnter()
{ 
	SpawnState::OnEnter();
	m_TargetPos = m_pCharacter->GetPosition();
	m_pCharacter->SetPosition(m_TargetPos - glm::vec3{ 0.f, m_HeightOffset, 0.f });
}

void EnemyDeathState::Update()
{
	m_AccumSec += dae::GameTime::GetInstance().GetDeltaTime();

	if (m_AccumSec >= m_RespawnDelay)
		return m_pCharacter->SetState(std::make_unique<EnemySpawnState>(m_pCharacter));

	
}
