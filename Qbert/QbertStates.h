#pragma once
#include "CharacterStates.h"

class QbertIdleState : public IdleState
{
public:
	QbertIdleState(CharacterComponent* pCharacter) : IdleState(pCharacter) {}
	virtual void HandleInput(MovementInfo) override;
	virtual void Notify(Character, Character) override;
};

class QbertJumpState : public JumpState
{
public:
	QbertJumpState(CharacterComponent* pCharacter, MovementInfo movementInfo) : JumpState(pCharacter, movementInfo) {}
	virtual void Update() override;
};

class QbertDeathState : public DeathState
{
public:
	QbertDeathState(CharacterComponent* pCharacter, const glm::vec3& startPos = glm::vec3{})
		: DeathState(pCharacter) 
		, m_StartPos{ startPos }
	{
	}
	virtual void Update() override;
	virtual void OnExit() override;
private:
	glm::vec3 m_StartPos{};
	float m_AccumSec{ 0.f };
	float m_RespawnDelay{ 1.f };
};

class QbertDiskState : public CharacterState
{
public:
	QbertDiskState(CharacterComponent* pCharacter) : CharacterState(pCharacter) {}
	virtual void Update() override;
	virtual void OnExit() override;
private:
	float m_AccumSec{};
	bool m_HasReachedTop{ false };
};

class QbertSpawnState : public SpawnState
{
public:
	QbertSpawnState(CharacterComponent* pCharacter) : SpawnState(pCharacter) {}
	virtual void Update() override;
};