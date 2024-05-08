#include "CharacterStates.h"
#include "CharacterComponent.h"
#include "GameTime.h"
#include "GameObject.h"
#include "ServiceLocator.h"
#include "Sounds.h"

std::unique_ptr<CharacterState> IdleState::HandleInput()
{
	return std::make_unique<JumpState>();
}

std::unique_ptr<CharacterState> IdleState::Update(CharacterComponent* )
{
	return nullptr;
}

void IdleState::OnEnter(CharacterComponent* pCharacter)
{
	switch (pCharacter->GetCharacter())
	{
	case Character::Qbert1:
	case Character::Qbert2:
		dae::ServiceLocator::GetSoundSystem().Play(dae::Sounds::QbertJump, 0.2f);
		break;
	case Character::Coily:
		break;
	default:
		break;
	}
}

void IdleState::OnExit(CharacterComponent*)
{
}

std::unique_ptr<CharacterState> JumpState::Update(CharacterComponent* pCharacter)
{
	glm::vec3 endPos{ m_StartPos };

	switch (m_MovementDirection)
	{
	case MovementDirection::Up:
		endPos += glm::vec3{ 16, -24, 0 };
		break;
	case MovementDirection::Down:
		endPos += glm::vec3{ -16, 24, 0 };
		break;
	case MovementDirection::Left:
		endPos += glm::vec3{ -16, -24, 0 };
		break;
	case MovementDirection::Right:
		endPos += glm::vec3{ 16, 24, 0 };
		break;
	default:
		assert(false);
		return nullptr;
	}

	glm::vec3 control{ m_StartPos };

	if (static_cast<int>(m_MovementDirection) < 2)
		control += (endPos - m_StartPos) * glm::vec3{ 0.2f, 1.f, 0.f };
	else
		control += (endPos - m_StartPos) * glm::vec3{ 1.f, 0.2f, 0.f };

	m_JumpLerpValue += dae::GameTime::GetInstance().GetDeltaTime() / m_JumpDuration;
	const float t = std::min(m_JumpLerpValue, 1.f);

	glm::vec3 currentPos = (1 - t) * (1 - t) * m_StartPos + 2 * (1 - t) * t * control + t * t * endPos;

	pCharacter->SetPosition(currentPos);

	if (t >= 1.f)
		pCharacter->MoveStateChanged->NotifyObservers(pCharacter->GetCharacter(), MovementState::End, m_MovementDirection);
	return nullptr;
}

void JumpState::OnEnter(CharacterComponent* pCharacter)
{
	m_StartPos = pCharacter->GetPosition();
}

void JumpState::OnExit(CharacterComponent*)
{
}
