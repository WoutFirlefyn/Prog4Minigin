#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "BaseComponent.h"
#include "Observer.h"

//-----------------------------------------------------
// DiscComponent Class									
//-----------------------------------------------------
namespace dae
{
	enum class Characters;
	enum class MovementState;
	enum class MovementDirection;
	class QbertComponent;
	class DiscComponent final : public BaseComponent, public Observer<Characters, MovementState, MovementDirection>
	{
	public:
		DiscComponent(GameObject* pGameObject, QbertComponent* pQbertComponent);
		~DiscComponent();			

		// -------------------------
		// Copy/move constructors and assignment operators
		// -------------------------    
		DiscComponent(const DiscComponent& other) = delete;
		DiscComponent(DiscComponent&& other) noexcept = delete;
		DiscComponent& operator=(const DiscComponent& other) = delete;
		DiscComponent& operator=(DiscComponent&& other)	noexcept = delete;

		//-------------------------------------------------
		// Member functions						
		//-------------------------------------------------
		virtual void Init() override;
		virtual void Update() override;

		virtual void Notify(Characters character, MovementState movementState, MovementDirection movementDirection) override;
		virtual void SubjectDestroyed(Subject<Characters, MovementState, MovementDirection>* pSubject) override;


	private:
		//-------------------------------------------------
		// Private member functions								
		//-------------------------------------------------


		//-------------------------------------------------
		// Datamembers								
		//-------------------------------------------------
		QbertComponent* m_pQbertComponent;
	};
}

 
