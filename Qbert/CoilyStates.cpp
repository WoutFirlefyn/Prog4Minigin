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
