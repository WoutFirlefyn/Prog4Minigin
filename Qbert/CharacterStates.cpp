#include "CharacterStates.h"
#include "CharacterComponent.h"
#include "GameTime.h"
#include "GameObject.h"
#include "LevelManagerComponent.h"

CharacterState::CharacterState(CharacterComponent* pCharacter)
	: m_pCharacter{ pCharacter }
{
}

void JumpState::OnEnter()
{
	m_StartPos = m_pCharacter->GetPosition(); 
	m_pCharacter->MoveStateChanged->NotifyObservers(m_pCharacter->GetCharacter(), MovementState::Start, m_MovementDirection);
}

void JumpState::OnExit()
{
	MovementState movementState{};

	switch (m_pCharacter->GetNextTileType())
	{
	case TileType::Tile:
		movementState = MovementState::End;
		break;
	case TileType::Disk:
		if (m_pCharacter->GetCharacter() == Character::Qbert1 || m_pCharacter->GetCharacter() == Character::Qbert2)
		{
			movementState = MovementState::Disk;
			break;
		}
	case TileType::None:
		movementState = MovementState::Fall;
		break;
	}

	m_pCharacter->MoveStateChanged->NotifyObservers(m_pCharacter->GetCharacter(), movementState, m_MovementDirection);
}

bool JumpState::Jump()
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
		return false;
	}

	glm::vec3 control{ m_StartPos };

	if (static_cast<int>(m_MovementDirection) < 2)
		control += (endPos - m_StartPos) * glm::vec3{ 0.2f, 1.f, 0.f };
	else
		control += (endPos - m_StartPos) * glm::vec3{ 1.f, 0.2f, 0.f };

	m_JumpLerpValue += dae::GameTime::GetInstance().GetDeltaTime() / m_JumpDuration;
	const float t = std::min(m_JumpLerpValue, 1.f);

	glm::vec3 currentPos = (1 - t) * (1 - t) * m_StartPos + 2 * (1 - t) * t * control + t * t * endPos;

	m_pCharacter->SetPosition(currentPos);

	return t >= 1.f;
}

void SpawnState::OnEnter()
{
	m_pCharacter->CharacterSpawned->NotifyObservers(m_pCharacter->GetCharacter());
}

void SpawnState::OnExit()
{
	m_pCharacter->MoveStateChanged->NotifyObservers(m_pCharacter->GetCharacter(), MovementState::End, MovementDirection::None);
}
