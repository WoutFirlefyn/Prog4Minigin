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
