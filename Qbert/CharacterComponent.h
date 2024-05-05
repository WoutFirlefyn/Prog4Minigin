#pragma once
#include "BaseComponent.h"
#include "Observer.h"
#include "glm/glm.hpp"

enum class MovementState
{
	Start,
	End
};

enum class MovementDirection
{
	Up,
	Left,
	Right,
	Down,
	None
};

enum class Character
{
	Qbert1,
	Qbert2,
	Coily,
	Ugg,
	Wrongway,
	Slick,
	Sam,
	None
};

class CharacterComponent : public dae::BaseComponent, public dae::Observer<Character, MovementState, MovementDirection>
{
public:
	CharacterComponent(dae::GameObject* pGameObject);	// Constructor
	virtual ~CharacterComponent() override;				// Destructor

	CharacterComponent(const CharacterComponent& other) = delete;
	CharacterComponent(CharacterComponent&& other) noexcept = delete;
	CharacterComponent& operator=(const CharacterComponent& other) = delete;
	CharacterComponent& operator=(CharacterComponent&& other) noexcept = delete;

	virtual void Init() override;
	virtual void Update() override;

	virtual void Notify(Character, MovementState movementState, MovementDirection movementDirection) = 0;

	bool IsMoving() const { return m_MovementDirection != MovementDirection::None; }

	std::unique_ptr<dae::Subject<Character, MovementState, MovementDirection>> MoveStateChanged;
protected:
	Character m_Character{ Character::None };
	MovementDirection m_MovementDirection{ MovementDirection::None };
	glm::vec3 m_StartPos{};
	float m_JumpLerpValue{ 0 };
	float m_JumpDuration{ 0.4f };
};

