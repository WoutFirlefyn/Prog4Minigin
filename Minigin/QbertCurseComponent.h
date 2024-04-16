#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "BaseComponent.h"
#include "Observer.h"

//-----------------------------------------------------
// QbertCurseComponent Class									
//-----------------------------------------------------
namespace dae
{
	enum class MovementState;
	enum class MovementDirection;
	enum class Character;
	class QbertComponent;
	class QbertCurseComponent final : public BaseComponent, public Observer<Character, MovementState, MovementDirection>
	{
	public:
		QbertCurseComponent(GameObject* pGameObject, QbertComponent* pQbertComponent);				// Constructor
		~QbertCurseComponent();				// Destructor

		// -------------------------
		// Copy/move constructors and assignment operators
		// -------------------------    
		QbertCurseComponent(const QbertCurseComponent& other) = delete;
		QbertCurseComponent(QbertCurseComponent&& other) noexcept = delete;
		QbertCurseComponent& operator=(const QbertCurseComponent& other) = delete;
		QbertCurseComponent& operator=(QbertCurseComponent&& other)	noexcept = delete;

		//-------------------------------------------------
		// Member functions						
		//-------------------------------------------------
		virtual void Init() override;
		virtual void Update() override;

		virtual void Notify(Character character, MovementState movementState, MovementDirection movementDirection) override;
		virtual void SubjectDestroyed(Subject<Character, MovementState, MovementDirection>* pSubject) override;


	private:
		//-------------------------------------------------
		// Private member functions								
		//-------------------------------------------------


		//-------------------------------------------------
		// Datamembers								
		//-------------------------------------------------
		QbertComponent* m_pQbertComponent;
		float m_AccumSec{ 0.f };
		float m_CurseDuration{ 1.f };

	};
}

 
