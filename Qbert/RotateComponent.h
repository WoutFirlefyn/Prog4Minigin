#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include <glm/glm.hpp>
#include "BaseComponent.h"

//-----------------------------------------------------
// RotateComponent Class									
//-----------------------------------------------------
class RotateComponent final : public dae::BaseComponent
{
public:
	RotateComponent(dae::GameObject* pGameObject);		// Constructor
	RotateComponent(dae::GameObject* pGameObject, float speed, float distance);		// Constructor
	virtual ~RotateComponent() = default;			// Destructor

	// -------------------------
	// Copy/move constructors and assignment operators
	// -------------------------    
	RotateComponent(const RotateComponent& other)					= delete;
	RotateComponent(RotateComponent&& other) noexcept				= delete;
	RotateComponent& operator=(const RotateComponent& other)		= delete;
	RotateComponent& operator=(RotateComponent&& other)	noexcept	= delete;

	//-------------------------------------------------
	// Member functions						
	//-------------------------------------------------
	virtual void Init() override;
	virtual void Update() override;

private:
	//-------------------------------------------------
	// Private member functions								
	//-------------------------------------------------
	glm::vec3 m_Center{};
	float m_StartAngle{};
	float m_CurrentAngle{};
	float m_Speed{ 5.f };
	float m_Distance{ 50.f };

	//-------------------------------------------------
	// Datamembers								
	//-------------------------------------------------


};


 
