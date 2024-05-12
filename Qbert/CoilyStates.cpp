#include "CoilyStates.h"
#include "CoilyComponent.h"
#include "GameTime.h"
#include "LevelManagerComponent.h"

void CoilyIdleState::Update()
{
	m_AccumSec += dae::GameTime::GetInstance().GetDeltaTime();

	if (m_AccumSec < m_TimeBetweenJumps)
		return;

	MovementDirection direction = static_cast<MovementDirection>((m_pCharacter->GetCharacter() == Character::Coily && CoilyComponent::IsEgg()) || (m_pCharacter->GetCharacter() == Character::Slick || m_pCharacter->GetCharacter() == Character::Sam) ? rand() % 2 + 2 : rand() % 4);
	return m_pCharacter->SetState(std::make_unique<CoilyJumpState>(m_pCharacter, direction));
}

void CoilyJumpState::Update()
{
	if (Jump())
	{
		switch (m_pCharacter->GetNextTileType())
		{
		case TileType::Tile:
			return m_pCharacter->SetState(std::make_unique<CoilyIdleState>(m_pCharacter));
		case TileType::Disk:
		case TileType::None:
			return m_pCharacter->SetState(std::make_unique<CoilyDeathState>(m_pCharacter));
		}
	}
}

void CoilySpawnState::Update()
{
	m_FallLerpValue += dae::GameTime::GetInstance().GetDeltaTime() / m_FallDuration;
	m_FallLerpValue = std::min(m_FallLerpValue, 1.f);
	m_pCharacter->SetPosition(m_TargetPos -  glm::vec3{ 0.f, m_HeightOffset * (1.f - m_FallLerpValue), 0.f });

	if (m_FallLerpValue >= 1.f)
		return m_pCharacter->SetState(std::make_unique<CoilyIdleState>(m_pCharacter));
}

void CoilySpawnState::OnEnter()
{ 
	SpawnState::OnEnter();
	m_TargetPos = m_pCharacter->GetPosition();
	m_pCharacter->SetPosition(m_TargetPos - glm::vec3{ 0.f, m_HeightOffset, 0.f });
}

void CoilySpawnState::OnExit()
{
	m_pCharacter->MoveStateChanged->NotifyObservers(m_pCharacter->GetCharacter(), MovementState::End, MovementDirection::None);
}

void CoilyDeathState::Update()
{
	m_AccumSec += dae::GameTime::GetInstance().GetDeltaTime();

	if (m_AccumSec >= m_RespawnDelay)
		return m_pCharacter->SetState(std::make_unique<CoilySpawnState>(m_pCharacter));

	
}
