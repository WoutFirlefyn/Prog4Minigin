#pragma once
#include "CharacterStates.h"

//class SlickSamIdleState : public IdleState
//{
//public:
//	SlickSamIdleState(CharacterComponent* pCharacter) : IdleState(pCharacter) {}
//	virtual std::unique_ptr<CharacterState> Update() override;
//private:
//	float m_AccumSec{ 0.f };
//	float m_TimeBetweenJumps{ 0.6f };
//};
//
//class SlickSamJumpState : public JumpState
//{
//public:
//	SlickSamJumpState(CharacterComponent* pCharacter, MovementDirection movementDirection) : JumpState(pCharacter, movementDirection) {}
//	virtual std::unique_ptr<CharacterState> Update() override;
//};
//
//class SlickSamSpawnState : public SpawnState
//{
//public:
//	SlickSamSpawnState(CharacterComponent* pCharacter) : SpawnState(pCharacter) {}
//	virtual std::unique_ptr<CharacterState> Update() override;
//	virtual void OnEnter() override;
//	virtual void OnExit() override;
//private:
//	glm::vec3 m_TargetPos{};
//	float m_HeightOffset{ 200.f };
//	float m_FallDuration{ 2.f };
//	float m_FallLerpValue{ 0.f };
//};
//
//class SlickSamDeathState : public DeathState
//{
//public:
//	SlickSamDeathState(CharacterComponent* pCharacter) : DeathState(pCharacter) {}
//	virtual std::unique_ptr<CharacterState> Update() override;
//};
