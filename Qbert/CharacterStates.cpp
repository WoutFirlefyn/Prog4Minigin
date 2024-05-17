#include "CharacterStates.h"
#include "CharacterComponent.h"
#include "GameTime.h"
#include "GameObject.h"
#include "LevelManagerComponent.h"

CharacterState::CharacterState(CharacterComponent* pCharacter)
	: m_pCharacter{ pCharacter }
{
}

JumpState::JumpState(CharacterComponent* pCharacter, MovementInfo movementInfo)
	: CharacterState(pCharacter)
	, m_MovementInfo{ movementInfo }
{
}

JumpState::~JumpState()
{
	m_pCharacterStartedJumping->RemoveObserver(this);
}

void JumpState::OnEnter()
{
	m_pCharacterStartedJumping = LevelManagerComponent::CharacterStartedJumping.get();
	m_pCharacterStartedJumping->AddObserver(this);
	m_StartPos = m_pCharacter->GetPosition(); 
	m_pCharacter->MoveStateChanged->NotifyObservers(m_pCharacter->GetCharacter(), m_MovementInfo);
}

void JumpState::OnExit()
{
	switch (m_NextTileType)
	{
	case TileType::Tile:
		m_MovementInfo.state = MovementState::End;
		break;
	case TileType::Disk:
		if (m_pCharacter->GetCharacter() == Character::Qbert1 || m_pCharacter->GetCharacter() == Character::Qbert2)
		{
			m_MovementInfo.state = MovementState::Disk;
			break;
		}
	case TileType::None:
		m_MovementInfo.state = MovementState::Fall;
		break;
	}

	m_pCharacter->MoveStateChanged->NotifyObservers(m_pCharacter->GetCharacter(), m_MovementInfo);
}

void JumpState::Notify(Character character, TileType tileType)
{
	if (character == m_pCharacter->GetCharacter())
		m_NextTileType = tileType;
}

void JumpState::SubjectDestroyed(dae::Subject<Character, TileType>* pSubject)
{
	if (pSubject == m_pCharacterStartedJumping)
		m_pCharacterStartedJumping = nullptr;
}

bool JumpState::Jump()
{
	glm::vec3 endPos{ m_StartPos + m_MovementInfo.vector };
	glm::vec3 control{ m_StartPos };

	if (static_cast<int>(m_MovementInfo.direction) < 2)
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
	MovementInfo movementInfo{};
	movementInfo.state = MovementState::End;
	m_pCharacter->MoveStateChanged->NotifyObservers(m_pCharacter->GetCharacter(), movementInfo);
}

bool SpawnState::Spawn()
{
	m_FallLerpValue += dae::GameTime::GetInstance().GetDeltaTime() / m_FallDuration;
	m_FallLerpValue = std::min(m_FallLerpValue, 1.f);
	m_pCharacter->SetPosition(m_TargetPos - glm::vec3{ 0.f, m_HeightOffset * (1.f - m_FallLerpValue), 0.f });

	return m_FallLerpValue >= 1.f;
}

IdleState::~IdleState()
{
	if (m_pCharactersCollide)
		m_pCharactersCollide->RemoveObserver(this);
}

void IdleState::OnEnter()
{
	m_pCharactersCollide = LevelManagerComponent::CharactersCollide.get();
	m_pCharactersCollide->AddObserver(this);
}

void IdleState::SubjectDestroyed(dae::Subject<Character, Character>* pSubject)
{
	if (m_pCharactersCollide == pSubject)
		m_pCharactersCollide = nullptr;
}
