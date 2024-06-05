#pragma once
#include "CharacterStates.h"

class CoilyComponent;
class CoilyIdleState : public IdleState
{
public:
	CoilyIdleState(CharacterComponent* pCharacter);
	virtual void Update() override;
	virtual void HandleInput(MovementInfo) override;
private:
	MovementInfo GetDirectionToNearestQbert() const;
	int CalculateTileDistance(const glm::ivec2& deltaPos) const;

	CoilyComponent* m_pCoilyComponent{ nullptr };
	float m_AccumSec{ 0.f };
	float m_TimeBetweenJumps{ 1.f };
};

class CoilyJumpState : public JumpState
{
public:
	CoilyJumpState(CharacterComponent* pCharacter, MovementInfo movementInfo) : JumpState(pCharacter, movementInfo) {}
	virtual void Update() override;
	virtual void OnEnter() override;
};

class CoilySpawnState : public SpawnState
{
public: 
	CoilySpawnState(CharacterComponent* pCharacter) : SpawnState(pCharacter) {}
	virtual void Update() override;
	virtual void OnEnter() override;
	virtual void OnExit() override;
};

class CoilyDeathState : public DeathState
{
public:
	CoilyDeathState(CharacterComponent* pCharacter) : DeathState(pCharacter) {}
	virtual void Update() override;
	virtual void OnEnter() override;
private:
	float m_AccumSec{};
	float m_RespawnDelay{ 5.f };
};

class CoilyResetState : public ResetState
{
public:
	CoilyResetState(CharacterComponent* pCharacter) : ResetState(pCharacter) {}
	virtual void Update() override;
};
