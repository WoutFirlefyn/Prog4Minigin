#include "UggWrongwayStates.h"
#include "UggWrongwayComponent.h"
#include "GameTime.h"
#include "GameObject.h"
#include "LevelManagerComponent.h"
#include "SpritesheetComponent.h"
#include "GraphicsComponent.h"
#include "ServiceLocator.h"
#include "Sounds.h"

void UggWrongwayIdleState::Update()
{
	if (m_AmountOfJumps == 7)
		return SetState(std::make_unique<UggWrongwayDeathState>(m_pCharacter));

	m_AccumSec += dae::GameTime::GetInstance().GetDeltaTime();
	
	if (m_AccumSec < m_TimeBetweenJumps)
		return;

	std::vector<MovementDirection> vDirections;
	if (GetGameObject()->GetComponent<UggWrongwayComponent>()->HasSpawnedLeft())
		vDirections = { MovementDirection::Up, MovementDirection::DiagonalRight };
	else
		vDirections = { MovementDirection::Left, MovementDirection::DiagonalLeft };
	return SetState(std::make_unique<UggWrongwayJumpState>(m_pCharacter, MovementInfo::GetMovementInfo(vDirections[rand() % vDirections.size()]), m_AmountOfJumps));
}

void UggWrongwayIdleState::OnExit()
{
	if (m_AmountOfJumps == 7)
	{
		MovementInfo movementInfo{};
		movementInfo.state = MovementState::Fall;
		m_pCharacter->MoveStateChanged->NotifyObservers(m_pCharacter->GetCharacter(), movementInfo);
	}
}

void UggWrongwayJumpState::Update()
{
	if (Jump())
		return SetState(std::make_unique<UggWrongwayIdleState>(m_pCharacter, m_AmountOfJumps));
}

void UggWrongwayJumpState::OnEnter()
{
	JumpState::OnEnter();
	GetGameObject()->GetComponent<dae::SpritesheetComponent>()->MoveSourceRect
	(
		static_cast<int>(m_MovementInfo.direction) % 10,
		static_cast<int>(m_pCharacter->GetCharacter()) - static_cast<int>(Character::Ugg)
	);
}

void UggWrongwayJumpState::OnExit()
{
	m_MovementInfo.state = MovementState::End;
	m_pCharacter->MoveStateChanged->NotifyObservers(m_pCharacter->GetCharacter(), m_MovementInfo);
}

void UggWrongwaySpawnState::Update()
{
	if (Spawn())
		return SetState(std::make_unique<UggWrongwayIdleState>(m_pCharacter, 0));
}

void UggWrongwaySpawnState::OnEnter()
{
	SpawnState::OnEnter();
	bool hasSpawnedLeft = GetGameObject()->GetComponent<UggWrongwayComponent>()->HasSpawnedLeft();
	GetGameObject()->GetComponent<dae::SpritesheetComponent>()->MoveSourceRect(!hasSpawnedLeft, static_cast<int>(m_pCharacter->GetCharacter()) - static_cast<int>(Character::Ugg));

	m_TargetPos = GetGameObject()->GetLocalPosition();
	m_StartPos = m_TargetPos + glm::vec3{ (hasSpawnedLeft ? -m_HeightOffset : m_HeightOffset), m_HeightOffset, 0.f };
	GetGameObject()->SetPosition(m_StartPos);
}

void UggWrongwaySpawnState::OnExit()
{
	MovementInfo movementInfo{};
	movementInfo.state = MovementState::End;
	m_pCharacter->MoveStateChanged->NotifyObservers(m_pCharacter->GetCharacter(), movementInfo);
}

void UggWrongwayDeathState::Update()
{
	m_AccumSec += dae::GameTime::GetInstance().GetDeltaTime();

	if (m_AccumSec >= m_RespawnDelay)
		return SetState(std::make_unique<UggWrongwaySpawnState>(m_pCharacter));
}

void UggWrongwayDeathState::OnEnter()
{
	GetGameObject()->GetComponent<dae::GraphicsComponent>()->ToggleRendering(false);
}

void UggWrongwayResetState::Update()
{
	if (m_EndResetState)
	{
		m_EndResetState = false;
		return SetState(std::make_unique<UggWrongwaySpawnState>(m_pCharacter));
	}
}
