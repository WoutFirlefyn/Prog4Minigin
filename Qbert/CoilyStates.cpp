#include "CoilyStates.h"
#include "CoilyComponent.h"
#include "GameTime.h"

std::unique_ptr<CharacterState> CoilyIdleState::Update()
{
	m_AccumSec += dae::GameTime::GetInstance().GetDeltaTime();

	if (m_AccumSec < m_TimeBetweenJumps)
		return nullptr;

	MovementDirection direction = static_cast<MovementDirection>(CoilyComponent::IsEgg() ? rand() % 2 + 2 : rand() % 4);

	return std::make_unique<CoilyJumpState>(m_pCharacter, direction);
}

std::unique_ptr<CharacterState> CoilyJumpState::Update()
{
	if (Jump())
	{
		return std::make_unique<CoilyIdleState>(m_pCharacter);
		//if (m_pCharacter->IsGoingToFall())
		//	return std::make_unique<QbertDeathState>(m_pCharacter);
		//else
	}
	return nullptr;
}

std::unique_ptr<CharacterState> CoilySpawnState::Update()
{
	m_FallLerpValue += dae::GameTime::GetInstance().GetDeltaTime() / m_FallDuration;
	m_FallLerpValue = std::min(m_FallLerpValue, 1.f);
	m_pCharacter->SetPosition(m_TargetPos -  glm::vec3{ 0.f, m_HeightOffset * (1.f - m_FallLerpValue), 0.f });

	if (m_FallLerpValue >= 1.f)
		return std::make_unique<CoilyIdleState>(m_pCharacter);

	return nullptr;
}

void CoilySpawnState::OnEnter()
{
	m_TargetPos = m_pCharacter->GetPosition();
	m_pCharacter->SetPosition(m_TargetPos - glm::vec3{ 0.f, m_HeightOffset, 0.f });
}

void CoilySpawnState::OnExit()
{
	m_pCharacter->MoveStateChanged->NotifyObservers(m_pCharacter->GetCharacter(), MovementState::End, MovementDirection::None);
}
