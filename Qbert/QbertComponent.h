#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "BaseComponent.h"
#include "Observer.h"
#include <memory>
#include <glm/glm.hpp>

//-----------------------------------------------------
// QbertComponent Class									
//-----------------------------------------------------
enum class MovementState
{
	Start,
	Moving,
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
	Sam
};

class QbertComponent final : public dae::BaseComponent, public dae::Observer<Character, MovementState, MovementDirection>, public dae::Observer<bool>, public dae::Observer<Character>
{
public:
	QbertComponent(dae::GameObject* pGameObject);	// Constructor
	virtual ~QbertComponent() override;				// Destructor

	// -------------------------
	// Copy/move constructors and assignment operators
	// -------------------------    
	QbertComponent(const QbertComponent& other) = delete;
	QbertComponent(QbertComponent&& other) noexcept = delete;
	QbertComponent& operator=(const QbertComponent& other) = delete;
	QbertComponent& operator=(QbertComponent&& other) noexcept = delete;

	//-------------------------------------------------
	// Member functions						
	//-------------------------------------------------
	virtual void Init() override;
	virtual void Update() override;

	void AddObserver(dae::Subject<bool>* pTileChangedSubject, dae::Subject<Character>* pCharacterFellSubject);
	virtual void Notify(Character character, MovementState movementState, MovementDirection movementDirection) override;
	virtual void Notify(Character character) override;
	void SubjectDestroyed(dae::Subject<Character>* pSubject);
	virtual void Notify(bool roundFinished) override;
	void SubjectDestroyed(dae::Subject<bool>* pSubject);

	bool IsMoving() const { return m_MovementDirection != MovementDirection::None; }
	void Die();
	int GetLives() const { return m_Lives; }

	std::unique_ptr<dae::Subject<int>> PlayerDied;
	std::unique_ptr<dae::Subject<Character, MovementState, MovementDirection>> MoveStateChanged;
private:
	dae::Subject<bool>* m_pTileChangedSubject{ nullptr };
	dae::Subject<Character>* m_pCharacterFellSubject{ nullptr };
	int m_Lives{ 3 };
	float m_AccumSec{ 0 };
	float m_JumpDuration{ 0.4f };
	MovementDirection m_MovementDirection{ MovementDirection::None };
	glm::vec3 m_StartPos{};
};


 
