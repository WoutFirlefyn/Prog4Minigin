#pragma once
#include "CharacterStates.h"

class UggWrongwayIdleState : public IdleState
{
public:
	UggWrongwayIdleState(CharacterComponent* pCharacter, int amountOfJumps) 
		: IdleState(pCharacter)
		, m_AmountOfJumps{ amountOfJumps }
	{
	}
	virtual void Update() override;
	virtual void OnExit() override;
private:
	int m_AmountOfJumps{};
	float m_AccumSec{ 0.f };
	float m_TimeBetweenJumps{ 1.f };
};

class UggWrongwayJumpState : public JumpState
{
public:
	UggWrongwayJumpState(CharacterComponent* pCharacter, MovementInfo movementInfo, int amountOfJumps)
		: JumpState(pCharacter, movementInfo)
		, m_AmountOfJumps{ ++amountOfJumps }
	{
	}
	virtual void Update() override;
	virtual void OnEnter() override;
	virtual void OnExit() override;
private:
	int m_AmountOfJumps{};
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

class UggWrongwayResetState : public ResetState
{
public:
	UggWrongwayResetState(CharacterComponent* pCharacter) : ResetState(pCharacter) {}
	virtual void Update() override;
};
