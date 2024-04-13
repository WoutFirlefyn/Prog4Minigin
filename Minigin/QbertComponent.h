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
	enum class ScoreType
	{
		TileCovered,
		LevelCleared
	};

	enum class MovementDirection
	{
		Up,
		Left,
		Right,
		Down,
		None
	};

	class QbertComponent final : public BaseComponent
	{
	public:
		QbertComponent(GameObject* pGameObject);				// Constructor
		virtual ~QbertComponent() override = default;			// Destructor

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
		virtual void Update() override;

		void Jump(MovementDirection direction);
		bool IsMoving() const { return m_MovementDirection != MovementDirection::None; }
		void Die();
		int GetLives() const { return m_Lives; }

		std::unique_ptr<Subject<>> PlayerDied;
		std::unique_ptr<Subject<MovementDirection>> PlayerMoved;
		std::unique_ptr<Subject<>> PlayerFinishedMoving;
	private:

		//-------------------------------------------------
		// Datamembers								
		//-------------------------------------------------
		int m_Lives{ 3 };
		float m_AccumSec{ 0 };
		float m_JumpDuration{ 0.5f };
		MovementDirection m_MovementDirection{ MovementDirection::None };
		glm::vec3 m_StartPos{};
	};
}

 
