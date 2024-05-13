#pragma once
#include <memory>
#include <glm/glm.hpp>
#include <iostream>
#include "CharacterComponent.h"

class CharacterState
{
public:
	virtual ~CharacterState() = default;
	virtual void HandleInput(MovementInfo);
	virtual void Update() {}
	virtual void OnEnter() {}
	virtual void OnExit() {}
	virtual void Notify(Character, Character) {}
protected:
	CharacterState(CharacterComponent* pCharacter);
	CharacterComponent* m_pCharacter{ nullptr };
};

class SpawnState : public CharacterState
{
public:
	SpawnState(CharacterComponent* pCharacter) : CharacterState(pCharacter) {}
	virtual void OnEnter() override;
	virtual void OnExit() override;
};

class IdleState : public CharacterState
{
public:
	IdleState(CharacterComponent* pCharacter) : CharacterState(pCharacter) {}
};

class JumpState : public CharacterState
{
public:
	JumpState(CharacterComponent* pCharacter, MovementInfo movementInfo);
	virtual void OnEnter() override;
	virtual void OnExit() override;
protected:
	bool Jump();
	float m_JumpDuration{ 0.4f };
	float m_JumpLerpValue{ 0.f };
	glm::vec3 m_StartPos{};
	MovementInfo m_MovementInfo;
};

class DeathState : public CharacterState
{
public:
	DeathState(CharacterComponent* pCharacter) : CharacterState(pCharacter) {}
	virtual void OnEnter() override { std::cout << "Died\n"; }
};

