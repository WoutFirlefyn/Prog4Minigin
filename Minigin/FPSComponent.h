#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "BaseComponent.h"

//-----------------------------------------------------
// FPSComponent Class									
//-----------------------------------------------------
namespace dae
{
	class GameObject;
	class TextComponent;

	class FPSComponent final : public BaseComponent
	{
	public:
		FPSComponent(GameObject* pGameObject);				// Constructor
		~FPSComponent();				// Destructor

		// -------------------------
		// Copy/move constructors and assignment operators
		// -------------------------    
		FPSComponent(const FPSComponent& other) = delete;
		FPSComponent(FPSComponent&& other) noexcept = delete;
		FPSComponent& operator=(const FPSComponent& other) = delete;
		FPSComponent& operator=(FPSComponent&& other)	noexcept = delete;

		//-------------------------------------------------
		// Member functions						
		//-------------------------------------------------
		virtual void Render() const override;
		virtual void Update(float deltaTime) override;



	private:
		//-------------------------------------------------
		// Private member functions								
		//-------------------------------------------------

		//-------------------------------------------------
		// Datamembers								
		//-------------------------------------------------
		TextComponent* m_pTextComponent{};
		float m_UpdateDelay{ 0.1f };
		float m_AccumTime{};

	};
}

 
