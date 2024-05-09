#include "QbertStates.h"
#include "QbertComponent.h"
#include "DiskComponent.h"
#include "GameTime.h"

std::unique_ptr<CharacterState> QbertIdleState::HandleInput(MovementDirection movementDirection)
{
	return std::make_unique<QbertJumpState>(m_pCharacter, movementDirection);
}

std::unique_ptr<CharacterState> QbertJumpState::Update()
{
	if (Jump())
	{
		// also disk state
		if (m_pCharacter->IsGoingToFall())
			return std::make_unique<QbertDeathState>(m_pCharacter, m_StartPos);
		else
			return std::make_unique<QbertIdleState>(m_pCharacter);
	}
	return nullptr;
}

std::unique_ptr<CharacterState> QbertDeathState::Update()
{
	m_AccumSec += dae::GameTime::GetInstance().GetDeltaTime();

	if (m_AccumSec < m_RespawnDelay)
		return nullptr;
	
	return std::make_unique<QbertIdleState>(m_pCharacter);
}

void QbertDeathState::OnExit()
{
	if (m_StartPos != glm::vec3{})
		m_pCharacter->SetPosition(m_StartPos);
}

std::unique_ptr<CharacterState> QbertDiskState::Update()
{
	m_AccumSec += dae::GameTime::GetInstance().GetDeltaTime();

	if (!m_HasReachedTop && m_AccumSec >= DiskComponent::m_TimeToReachTop)
	{
		m_AccumSec -= DiskComponent::m_TimeToReachTop;
		m_HasReachedTop = true;
	}
	return nullptr; 

}
