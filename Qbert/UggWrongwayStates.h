#pragma once
#include "CharacterStates.h"

class UggWrongwayIdleState : public IdleState
{
public:
	UggWrongwayIdleState(CharacterComponent* pCharacter) : IdleState(pCharacter) {}
	virtual void Update() override;
private:
	float m_AccumSec{ 0.f };
	float m_TimeBetweenJumps{ 1.f };
};

class UggWrongwayJumpState : public JumpState
{
public:
	UggWrongwayJumpState(CharacterComponent* pCharacter, MovementInfo movementInfo) : JumpState(pCharacter, movementInfo) {}
	virtual void Update() override;
	virtual void OnEnter() override;
};

class UggWrongwaySpawnState : public SpawnState
{
public:
	UggWrongwaySpawnState(CharacterComponent* pCharacter) : SpawnState(pCharacter) {}
	virtual void Update() override;
	virtual void OnEnter() override;
	virtual void OnExit() override;
};

class UggWrongwayDeathState : public DeathState
{
public:
	UggWrongwayDeathState(CharacterComponent* pCharacter) : DeathState(pCharacter) {}
	virtual void Update() override;
	virtual void OnEnter() override;
private:
	float m_AccumSec{};
	float m_RespawnDelay{ 5.f };
};
