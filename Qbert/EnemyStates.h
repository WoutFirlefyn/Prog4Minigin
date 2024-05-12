#pragma once
#include "CharacterStates.h"

class EnemyIdleState : public IdleState
{
public:
	EnemyIdleState(CharacterComponent* pCharacter) : IdleState(pCharacter) {}
	virtual void Update() override;
	virtual void Notify(Character, Character) override;
private:
	float m_AccumSec{ 0.f };
	float m_TimeBetweenJumps{ 0.6f };
};

class EnemyJumpState : public JumpState
{
public:
	EnemyJumpState(CharacterComponent* pCharacter, MovementDirection movementDirection) : JumpState(pCharacter, movementDirection) {}
	virtual void Update() override;
};

class EnemySpawnState : public SpawnState
{
public: 
	EnemySpawnState(CharacterComponent* pCharacter) : SpawnState(pCharacter) {}
	virtual void Update() override;
	virtual void OnEnter() override;
private:
	glm::vec3 m_TargetPos{};
	float m_HeightOffset{ 200.f };
	float m_FallDuration{ 2.f };
	float m_FallLerpValue{ 0.f };
};

class EnemyDeathState : public DeathState
{
public:
	EnemyDeathState(CharacterComponent* pCharacter) : DeathState(pCharacter) {}
	virtual void Update() override;
private:
	float m_AccumSec{};
	float m_RespawnDelay{ 5.f };
};
