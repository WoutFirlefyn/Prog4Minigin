#pragma once
#include "CharacterStates.h"

class CoilyIdleState : public IdleState
{
public:
	CoilyIdleState(CharacterComponent* pCharacter) : IdleState(pCharacter) {}
	virtual std::unique_ptr<CharacterState> Update() override;
private:
	float m_AccumSec{ 0.f };
	float m_TimeBetweenJumps{ 0.6f };
};

class CoilyJumpState : public JumpState
{
public:
	CoilyJumpState(CharacterComponent* pCharacter, MovementDirection movementDirection) : JumpState(pCharacter, movementDirection) {}
	virtual std::unique_ptr<CharacterState> Update() override;
};

class CoilySpawnState : public SpawnState
{
public: 
	CoilySpawnState(CharacterComponent* pCharacter) : SpawnState(pCharacter) {}
	virtual std::unique_ptr<CharacterState> Update() override;
	virtual void OnEnter() override;
	virtual void OnExit() override;
private:
	glm::vec3 m_TargetPos{};
	float m_HeightOffset{ 200.f };
	float m_FallDuration{ 2.f };
	float m_FallLerpValue{ 0.f };
};

class CoilyDeathState : public DeathState
{
public:
	CoilyDeathState(CharacterComponent* pCharacter) : DeathState(pCharacter) {}
	virtual std::unique_ptr<CharacterState> Update() override;
private:
	float m_AccumSec{};
	float m_RespawnDelay{ 5.f };
};
