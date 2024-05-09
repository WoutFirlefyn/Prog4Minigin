#pragma once
#include "CharacterStates.h"

class QbertIdleState : public IdleState
{
public:
	QbertIdleState(CharacterComponent* pCharacter) : IdleState(pCharacter) {}
	virtual std::unique_ptr<CharacterState> HandleInput(MovementDirection movementDirection) override;
};

class QbertJumpState : public JumpState
{
public:
	QbertJumpState(CharacterComponent* pCharacter, MovementDirection movementDirection) : JumpState(pCharacter, movementDirection) {}
	virtual std::unique_ptr<CharacterState> Update() override;
};

class QbertDeathState : public DeathState
{
public:
	QbertDeathState(CharacterComponent* pCharacter, const glm::vec3& startPos = glm::vec3{})
		: DeathState(pCharacter) 
		, m_StartPos{ startPos }
	{
	}
	virtual std::unique_ptr<CharacterState> Update() override;
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
	virtual std::unique_ptr<CharacterState> Update() override;
private:
	float m_AccumSec{};
	bool m_HasReachedTop{ false };
};