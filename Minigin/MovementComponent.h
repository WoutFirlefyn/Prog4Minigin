#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include <glm/glm.hpp>
#include "BaseComponent.h"

//-----------------------------------------------------
// MovementComponent Class									
//-----------------------------------------------------
enum class MovementDir
{
	Up,
	Down,
	Left,
	Right
};

namespace dae
{
	class GameObject;
	class MovementComponent final : public BaseComponent
	{
	public:
		MovementComponent(GameObject* pGameObject, float movementSpeed = 50.f);				// Constructor
		virtual ~MovementComponent() override = default;				// Destructor

		// -------------------------
		// Copy/move constructors and assignment operators
		// -------------------------    
		MovementComponent(const MovementComponent& other)					= delete;
		MovementComponent(MovementComponent&& other) noexcept				= delete;
		MovementComponent& operator=(const MovementComponent& other)		= delete;
		MovementComponent& operator=(MovementComponent&& other)	noexcept	= delete;

		//-------------------------------------------------
		// Member functions						
		//-------------------------------------------------
		virtual void Init() override;
		virtual void Render() const override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void RenderGUI() override;

		void Move(MovementDir dir);

	private:
		//-------------------------------------------------
		// Private member functions								
		//-------------------------------------------------


		//-------------------------------------------------
		// Datamembers								
		//-------------------------------------------------
		float m_MovementSpeed{};
		glm::vec3 m_Velocity{};
	};
}

 
