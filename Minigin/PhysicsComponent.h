#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "BaseComponent.h"

//-----------------------------------------------------
// PhysicsComponent Class									
//-----------------------------------------------------
namespace dae
{
	class PhysicsComponent : public BaseComponent
	{
	public:
		PhysicsComponent();					// Constructor
		virtual ~PhysicsComponent();		// Destructor

		// -------------------------
		// Copy/move constructors and assignment operators
		// -------------------------    
		PhysicsComponent(const PhysicsComponent& other) = delete;
		PhysicsComponent(PhysicsComponent&& other) noexcept = delete;
		PhysicsComponent& operator=(const PhysicsComponent& other) = delete;
		PhysicsComponent& operator=(PhysicsComponent&& other) noexcept = delete;

		//-------------------------------------------------
		// Member functions						
		//-------------------------------------------------
		virtual void Update(float deltaTime) = 0;

	private:
		//-------------------------------------------------
		// Private member functions								
		//-------------------------------------------------


		//-------------------------------------------------
		// Datamembers								
		//-------------------------------------------------


	};
}

 
