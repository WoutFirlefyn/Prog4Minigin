#pragma once
#include <memory>
#include <glm/glm.hpp>
class CharacterComponent;
enum class MovementDirection;
class CharacterState
{
public:
	CharacterState(CharacterComponent* pCharacter);
	virtual ~CharacterState() = default;
	virtual std::unique_ptr<CharacterState> HandleInput(MovementDirection) { return nullptr; }
	virtual std::unique_ptr<CharacterState> Update() = 0;
	virtual void OnEnter() {}
	virtual void OnExit() {}
	virtual bool IsMoving() const { return true; }
protected:
	CharacterComponent* m_pCharacter{ nullptr };
};

class SpawnState : public CharacterState
{
public:
	SpawnState(CharacterComponent* pCharacter) : CharacterState(pCharacter) {}
	virtual std::unique_ptr<CharacterState> Update() override;
	virtual void OnEnter() override;
	virtual void OnExit() override;
};

class IdleState : public CharacterState
{
public:
	IdleState(CharacterComponent* pCharacter) : CharacterState(pCharacter) {}
	virtual std::unique_ptr<CharacterState> Update() override;
	virtual void OnEnter() override;
	virtual void OnExit() override;
	virtual bool IsMoving() const override { return false; }
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
	virtual std::unique_ptr<CharacterState> Update() override;
	virtual void OnEnter() override;
	virtual void OnExit() override;
};

