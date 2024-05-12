#pragma once
#include <memory>
#include <glm/glm.hpp>
#include <iostream>
class CharacterComponent;
enum class MovementDirection;
enum class Character;
class CharacterState
{
public:
	CharacterState(CharacterComponent* pCharacter);
	virtual ~CharacterState() = default;
	virtual void HandleInput(MovementDirection) {}
	virtual void Update() {}
	virtual void OnEnter() {}
	virtual void OnExit() {}
	virtual void Notify(Character, Character) {}
protected:
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
	JumpState(CharacterComponent* pCharacter, MovementDirection movementDirection)
		: CharacterState(pCharacter)
		, m_MovementDirection{ movementDirection }
	{
	}
	virtual void OnEnter() override;
	virtual void OnExit() override;
protected:
	bool Jump();
	float m_JumpDuration{ 0.4f };
	float m_JumpLerpValue{ 0.f };
	glm::vec3 m_StartPos{};
	MovementDirection m_MovementDirection{};
};

class DeathState : public CharacterState
{
public:
	DeathState(CharacterComponent* pCharacter) : CharacterState(pCharacter) {}
	virtual void OnEnter() override { std::cout << "Died\n"; }
};

