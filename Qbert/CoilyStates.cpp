#include "CoilyStates.h"
#include "CoilyComponent.h"
#include "GameTime.h"
#include "GameObject.h"
#include "LevelManagerComponent.h"
#include "SpritesheetComponent.h"
#include "GraphicsComponent.h"
#include "Sounds.h"
#include "ServiceLocator.h"

void CoilyIdleState::Update()
{
	m_AccumSec += dae::GameTime::GetInstance().GetDeltaTime();

	if (m_AccumSec < m_TimeBetweenJumps)
		return;
	MovementInfo movementInfo{};
	if (CoilyComponent::IsEgg())
		movementInfo = MovementInfo::GetMovementInfo(static_cast<MovementDirection>(rand() % 2 + 2));
	else
		movementInfo = GetLevelManagerComponent()->GetDirectionToNearestQbert();
	return SetState(std::make_unique<CoilyJumpState>(m_pCharacter, movementInfo));
}

void CoilyJumpState::Update()
{
	if (Jump())
	{
		switch (m_NextTileType)
		{
		case TileType::Tile:
			return SetState(std::make_unique<CoilyIdleState>(m_pCharacter));
		case TileType::Disk:
		case TileType::None:
			return SetState(std::make_unique<CoilyDeathState>(m_pCharacter));
		}
	}
}

void CoilyJumpState::OnEnter()
{
	JumpState::OnEnter();
	GetGameObject()->GetComponent<dae::SpritesheetComponent>()->MoveSourceRect(CoilyComponent::IsEgg() ? 4 : static_cast<int>(m_MovementInfo.direction), 1);
}

void CoilySpawnState::Update()
{
	if(Spawn())
		return SetState(std::make_unique<CoilyIdleState>(m_pCharacter));
}

void CoilySpawnState::OnEnter()
{ 
	SpawnState::OnEnter();
	GetGameObject()->GetComponent<dae::SpritesheetComponent>()->MoveSourceRect(4, 1);

	m_TargetPos = GetGameObject()->GetLocalPosition();
	m_StartPos = m_TargetPos - glm::vec3{ 0.f, m_HeightOffset, 0.f };
	GetGameObject()->SetPosition(m_StartPos);
}

void CoilySpawnState::OnExit()
{
	MovementInfo movementInfo{};
	movementInfo.state = MovementState::End;
	m_pCharacter->MoveStateChanged->NotifyObservers(m_pCharacter->GetCharacter(), movementInfo);
}

void CoilyDeathState::Update()
{
	m_AccumSec += dae::GameTime::GetInstance().GetDeltaTime();

	if (m_AccumSec >= m_RespawnDelay)
		return SetState(std::make_unique<CoilySpawnState>(m_pCharacter));
}

void CoilyDeathState::OnEnter()
{
	dae::ServiceLocator::GetSoundSystem().Play(dae::Sounds::CoilyFall, 0.2f);
	GetGameObject()->GetComponent<dae::GraphicsComponent>()->ToggleRendering(false);
}

void CoilyResetState::Update()
{
	if (Wait())
		return SetState(std::make_unique<CoilySpawnState>(m_pCharacter));
}
