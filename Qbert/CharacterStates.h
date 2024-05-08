#pragma once
#include <memory>
#include <glm/glm.hpp>
class CharacterComponent;
enum class MovementDirection;
class CharacterState
{
public:
	virtual ~CharacterState() = default;
	virtual std::unique_ptr<CharacterState> HandleInput() { return nullptr; }
	virtual std::unique_ptr<CharacterState> Update(CharacterComponent* pCharacter) = 0;
	virtual void OnEnter(CharacterComponent* pCharacter) = 0;
	virtual void OnExit(CharacterComponent* pCharacter) = 0;
};

class IdleState : public CharacterState
{
public:
	virtual std::unique_ptr<CharacterState> HandleInput() override;
	virtual std::unique_ptr<CharacterState> Update(CharacterComponent* pCharacter) override;
	virtual void OnEnter(CharacterComponent* pCharacter) override;
	virtual void OnExit(CharacterComponent* pCharacter) override;
private:
};

class JumpState : public CharacterState
{
public:
	virtual std::unique_ptr<CharacterState> Update(CharacterComponent* pCharacter) override;
	virtual void OnEnter(CharacterComponent* pCharacter) override;
	virtual void OnExit(CharacterComponent* pCharacter) override;
private:
	float m_JumpDuration{ 0.4f };
	float m_JumpLerpValue{ 0.f };
	glm::vec3 m_StartPos{};
	MovementDirection m_MovementDirection{};
};

