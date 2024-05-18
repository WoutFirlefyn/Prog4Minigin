#pragma once
#include "CharacterStates.h"

class SlickSamIdleState : public IdleState
{
public:
	SlickSamIdleState(CharacterComponent* pCharacter) : IdleState(pCharacter) {}
	virtual void Update() override;
	virtual void Notify(Character, Character) override;
private:
	float m_AccumSec{ 0.f };
	float m_TimeBetweenJumps{ 1.f };
};

class SlickSamJumpState : public JumpState
{
public:
	SlickSamJumpState(CharacterComponent* pCharacter, MovementInfo movementInfo) : JumpState(pCharacter, movementInfo) {}
	virtual void Update() override;
	virtual void OnEnter() override;
};

class SlickSamSpawnState : public SpawnState
{
public: 
	SlickSamSpawnState(CharacterComponent* pCharacter) : SpawnState(pCharacter) {}
	virtual void Update() override;
	virtual void OnEnter() override;
	virtual void OnExit() override;
};

class SlickSamDeathState : public DeathState
{
public:
	SlickSamDeathState(CharacterComponent* pCharacter) : DeathState(pCharacter) {}
	virtual void Update() override;
	virtual void OnEnter() override;
private:
	float m_AccumSec{};
	float m_RespawnDelay{ 5.f };
};
