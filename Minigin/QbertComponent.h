#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "BaseComponent.h"
#include "Observer.h"
#include <memory>

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
		Down,
		Left,
		Right
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
		void Die();
		int GetLives() const { return m_Lives; }
		void GainScore(ScoreType type);		
		int GetScore() const { return m_Score; }

		std::unique_ptr<Subject<>> PlayerDied;
		std::unique_ptr<Subject<>> ScoreChanged;
		std::unique_ptr<Subject<MovementDirection>> PlayerMoved;
	private:
		//-------------------------------------------------
		// Datamembers								
		//-------------------------------------------------
		int m_Lives{ 3 };
		int m_Score{ 0 };
	};
}

 
