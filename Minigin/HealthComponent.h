#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "BaseComponent.h"
#include "Observer.h"

//-----------------------------------------------------
// HealthComponent Class									
//-----------------------------------------------------
namespace dae
{
	class HealthComponent final : public BaseComponent, public Subject
	{
	public:
		HealthComponent(GameObject* pGameObject);				// Constructor
		virtual ~HealthComponent() override = default;			// Destructor

		// -------------------------
		// Copy/move constructors and assignment operators
		// -------------------------    
		HealthComponent(const HealthComponent& other) = delete;
		HealthComponent(HealthComponent&& other) noexcept = delete;
		HealthComponent& operator=(const HealthComponent& other) = delete;
		HealthComponent& operator=(HealthComponent&& other)	noexcept = delete;

		//-------------------------------------------------
		// Member functions						
		//-------------------------------------------------
		virtual void Init() override;
		virtual void Render() const override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void RenderGUI() override;

		void LoseALife();
		int GetLives() const { return m_Lives; }

	private:
		//-------------------------------------------------
		// Private member functions								
		//-------------------------------------------------


		//-------------------------------------------------
		// Datamembers								
		//-------------------------------------------------
		int m_Lives{ 3 };
	};
}

 
