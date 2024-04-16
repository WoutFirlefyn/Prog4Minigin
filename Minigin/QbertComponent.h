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
namespace dae
{
	enum class MovementState
	{
		Start,
		Moving,
		End,
		Falling
	};

	enum class MovementDirection
	{
		Up,
		Left,
		Right,
		Down,
		None
	};

	enum class Characters
	{
		Qbert1,
		Qbert2,
		Coily,
		Ugg,
		Wrongway,
		Slick,
		Sam
	};

	class QbertComponent final : public BaseComponent, public Observer<Characters, MovementState, MovementDirection>, public Observer<bool>
	{
	public:
		QbertComponent(GameObject* pGameObject);				// Constructor
		virtual ~QbertComponent() override;			// Destructor

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

		virtual void Notify(Characters character, MovementState movementState, MovementDirection movementDirection) override;
		virtual void Notify(bool roundFinished) override;

		bool IsMoving() const { return m_MovementDirection != MovementDirection::None; }
		void Die();
		int GetLives() const { return m_Lives; }

		std::unique_ptr<Subject<>> PlayerDied;
		std::unique_ptr<Subject<Characters, MovementState, MovementDirection>> MoveStateChanged;
		bool m_IsFalling{ false };
	private:

		//-------------------------------------------------
		// Datamembers								
		//-------------------------------------------------
		int m_Lives{ 3 };
		float m_AccumSec{ 0 };
		float m_JumpDuration{ 0.4f };
		MovementDirection m_MovementDirection{ MovementDirection::None };
		glm::vec3 m_StartPos{};
	};
}

 
