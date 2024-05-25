#include "CharacterStates.h"
#include "CharacterComponent.h"
#include "GameTime.h"
#include "GameObject.h"
#include "LevelManagerComponent.h"
#include "GraphicsComponent.h"

CharacterState::CharacterState(CharacterComponent* pCharacter)
	: m_pCharacter{ pCharacter }
{
}

void CharacterState::SetState(std::unique_ptr<CharacterState>&& pNewState)
{
	m_pCharacter->SetState(std::move(pNewState));
}

dae::GameObject* CharacterState::GetGameObject() const
{
	return m_pCharacter->GetGameObject();
}

LevelManagerComponent* CharacterState::GetLevelManagerComponent() const
{
	return m_pCharacter->GetLevelManagerComponent();
}

JumpState::JumpState(CharacterComponent* pCharacter, MovementInfo movementInfo)
	: CharacterState(pCharacter)
	, m_MovementInfo{ movementInfo }
{
}

void JumpState::OnEnter()
{
	m_NextTileType = GetLevelManagerComponent()->GetNextTileType(m_pCharacter->GetCharacter(), m_MovementInfo);
	m_StartPos = GetGameObject()->GetLocalPosition(); 
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
	case TileType::None:
		m_MovementInfo.state = MovementState::Fall;
		break;
	}

	m_pCharacter->MoveStateChanged->NotifyObservers(m_pCharacter->GetCharacter(), m_MovementInfo);
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

	GetGameObject()->SetPosition(currentPos);

	return t >= 1.f;
}

void SpawnState::OnEnter()
{
	m_pCharacter->CharacterSpawned->NotifyObservers(m_pCharacter->GetCharacter(), GetGameObject());
	GetGameObject()->GetComponent<dae::GraphicsComponent>()->ToggleRendering(true);
}

bool SpawnState::Spawn()
{
	m_FallLerpValue += dae::GameTime::GetInstance().GetDeltaTime() / m_FallDuration;
	m_FallLerpValue = std::min(m_FallLerpValue, 1.f);
	GetGameObject()->SetPosition(m_StartPos - (m_StartPos - m_TargetPos) * m_FallLerpValue);

	return m_FallLerpValue >= 1.f;
}

IdleState::IdleState(CharacterComponent* pCharacter)
	: CharacterState(pCharacter)
	, m_pCharactersCollide{ LevelManagerComponent::CharactersCollide.get() }
{
	m_pCharactersCollide->AddObserver(this);
}

IdleState::~IdleState()
{
	if (m_pCharactersCollide)
		m_pCharactersCollide->RemoveObserver(this);
}

void IdleState::SubjectDestroyed(dae::Subject<Character, Character>* pSubject)
{
	if (m_pCharactersCollide == pSubject)
		m_pCharactersCollide = nullptr;
}
