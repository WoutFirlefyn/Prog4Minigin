#include "SlickSamStates.h"
#include "CoilyComponent.h"
#include "GameTime.h"
#include "GameObject.h"
#include "LevelManagerComponent.h"
#include "SpritesheetComponent.h"
#include "GraphicsComponent.h"
#include "ServiceLocator.h"
#include "Sounds.h"

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
	Character slickSamCharacter = m_pCharacter->GetCharacter();

	if (character != slickSamCharacter && otherCharacter != slickSamCharacter)
		return;

	if (otherCharacter == slickSamCharacter)
		std::swap(character, otherCharacter);

	switch (otherCharacter)
	{
	case Character::Qbert1:
	case Character::Qbert2:
	{
		MovementInfo movementInfo{};
		movementInfo.state = MovementState::Fall;
		m_pCharacter->MoveStateChanged->NotifyObservers(slickSamCharacter, movementInfo);
		dae::ServiceLocator::GetSoundSystem().Play(dae::Sounds::SlickSamCaught, 0.6f);
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
		m_NextTileType = GetLevelManagerComponent()->GetTileType(m_pCharacter->GetCharacter(), m_MovementInfo);
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

void SlickSamJumpState::OnEnter()
{
	JumpState::OnEnter();
	GetGameObject()->GetComponent<dae::SpritesheetComponent>()->MoveSourceRect
	(
		static_cast<int>(m_MovementInfo.direction) - 2,
		static_cast<int>(m_pCharacter->GetCharacter()) - static_cast<int>(Character::Slick)
	);
}

void SlickSamSpawnState::Update()
{
	if (Spawn())
		return SetState(std::make_unique<SlickSamIdleState>(m_pCharacter));
}

void SlickSamSpawnState::OnEnter()
{ 
	SpawnState::OnEnter();
	GetGameObject()->GetComponent<dae::SpritesheetComponent>()->MoveSourceRect(0, static_cast<int>(m_pCharacter->GetCharacter()) - static_cast<int>(Character::Slick));

	m_TargetPos = GetGameObject()->GetLocalPosition();
	m_StartPos = m_TargetPos - glm::vec3{ 0.f, m_HeightOffset, 0.f };
	GetGameObject()->SetPosition(m_StartPos);
}

void SlickSamSpawnState::OnExit()
{
	MovementInfo movementInfo{};
	movementInfo.state = MovementState::End;
	m_pCharacter->MoveStateChanged->NotifyObservers(m_pCharacter->GetCharacter(), movementInfo);
}

void SlickSamDeathState::Update()
{
	m_AccumSec += dae::GameTime::GetInstance().GetDeltaTime();

	if (m_AccumSec >= m_RespawnDelay)
		return SetState(std::make_unique<SlickSamSpawnState>(m_pCharacter));
}

void SlickSamDeathState::OnEnter()
{
	GetGameObject()->GetComponent<dae::GraphicsComponent>()->ToggleRendering(false);
}

void SlickSamResetState::Update()
{
	if (m_NewRoundStarted)
	{
		m_NewRoundStarted = false;
		return SetState(std::make_unique<SlickSamSpawnState>(m_pCharacter));
	}
}
