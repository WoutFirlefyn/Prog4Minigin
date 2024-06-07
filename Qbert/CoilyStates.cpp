#include "CoilyStates.h"
#include "CoilyComponent.h"
#include "GameTime.h"
#include "GameObject.h"
#include "LevelManagerComponent.h"
#include "SpritesheetComponent.h"
#include "GraphicsComponent.h"
#include "Sounds.h"
#include "ServiceLocator.h"
#include "Game.h"

CoilyIdleState::CoilyIdleState(CharacterComponent* pCharacter) : IdleState(pCharacter) 
{
	m_pCoilyComponent = dynamic_cast<CoilyComponent*>(m_pCharacter);
	assert(m_pCoilyComponent);
}

void CoilyIdleState::Update()
{
	m_AccumSec += dae::GameTime::GetInstance().GetDeltaTime();

	if (m_AccumSec < m_TimeBetweenJumps)
		return;
	MovementInfo movementInfo{};
	if (m_pCoilyComponent->IsEgg())
		movementInfo = MovementInfo::GetMovementInfo(static_cast<MovementDirection>(rand() % 2 + 2));
	else
	{
		if (Game::GetInstance().GetCurrentSceneType() == SceneType::Versus)
			return;
		movementInfo = GetDirectionToNearestQbert();
	}

	return SetState(std::make_unique<CoilyJumpState>(m_pCharacter, movementInfo));
}

void CoilyIdleState::HandleInput(MovementInfo movementInfo)
{
	if (Game::GetInstance().GetCurrentSceneType() == SceneType::Versus
		&& !m_pCoilyComponent->IsEgg()
		&& m_AccumSec >= m_TimeBetweenJumps)
		return SetState(std::make_unique<CoilyJumpState>(m_pCharacter, movementInfo));
}

MovementInfo CoilyIdleState::GetDirectionToNearestQbert() const
{
	auto coilyTilePair = GetLevelManagerComponent()->GetCharacter(Character::Coily);
	auto qbert1TilePair = GetLevelManagerComponent()->GetCharacter(Character::Qbert1);
	auto qbert2TilePair = GetLevelManagerComponent()->GetCharacter(Character::Qbert2);

	if (coilyTilePair.first == Character::None || qbert1TilePair.first == Character::None)
	{
		assert(false);
		return MovementInfo{};
	}

	glm::ivec2 deltaTileIdx = coilyTilePair.second.tileIndex - qbert1TilePair.second.tileIndex;

	if (CalculateTileDistance(deltaTileIdx) == 0)
		deltaTileIdx = coilyTilePair.second.tileIndex - qbert1TilePair.second.previousTileIndex;

	if (qbert2TilePair.first != Character::None)
	{
		glm::ivec2 deltaTileIdx2 = coilyTilePair.second.tileIndex - qbert2TilePair.second.tileIndex;
		if (CalculateTileDistance(deltaTileIdx2) == 0)
			deltaTileIdx2 = coilyTilePair.second.tileIndex - qbert2TilePair.second.previousTileIndex;
		if (CalculateTileDistance(deltaTileIdx) > CalculateTileDistance(deltaTileIdx2))
			deltaTileIdx = deltaTileIdx2;
	}

	MovementInfo option1 = MovementInfo::GetMovementInfo((deltaTileIdx.x < 0) ? MovementDirection::Right : MovementDirection::Left);
	MovementInfo option2 = MovementInfo::GetMovementInfo((deltaTileIdx.y < 0) ? MovementDirection::Down  : MovementDirection::Up);

	if (std::abs(deltaTileIdx.x) > std::abs(deltaTileIdx.y))
		return option1;
	else if (std::abs(deltaTileIdx.x) < std::abs(deltaTileIdx.y))
		return option2;
	else
	{
		if (rand() % 2 == 0)
			std::swap(option1, option2);

		const auto& tiles = GetLevelManagerComponent()->GetTiles();
		if (tiles.find(coilyTilePair.second.tileIndex + option1.indexOffset) != tiles.end())
			return option1;
		else
			return option2;
	}
}

int CoilyIdleState::CalculateTileDistance(const glm::ivec2& deltaPos) const
{
	return std::abs(deltaPos.x) + std::abs(deltaPos.y);
}

void CoilyJumpState::Update()
{
	if (Jump())
	{
		m_NextTileType = GetLevelManagerComponent()->GetTileType(m_pCharacter->GetCharacter(), m_MovementInfo);
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

	CoilyComponent* pCoilyComponent = dynamic_cast<CoilyComponent*>(m_pCharacter);
	assert(pCoilyComponent);
	GetGameObject()->GetComponent<dae::SpritesheetComponent>()->MoveSourceRect(pCoilyComponent->IsEgg() ? 4 : static_cast<int>(m_MovementInfo.direction), 1);
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
	if (m_EndResetState)
	{
		m_EndResetState = false;
		return SetState(std::make_unique<CoilySpawnState>(m_pCharacter));
	}
}
