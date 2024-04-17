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
	enum class Character;
	enum class MovementState;
	enum class MovementDirection;
	class QbertComponent;
	class DiskComponent final : public BaseComponent, public Observer<Character, MovementState, MovementDirection>
	{
	public:
		DiskComponent(GameObject* pGameObject, QbertComponent* pQbertComponent);
		~DiskComponent();			

		// -------------------------
		// Copy/move constructors and assignment operators
		// -------------------------    
		DiskComponent(const DiskComponent& other) = delete;
		DiskComponent(DiskComponent&& other) noexcept = delete;
		DiskComponent& operator=(const DiskComponent& other) = delete;
		DiskComponent& operator=(DiskComponent&& other)	noexcept = delete;

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
	};
}

 
