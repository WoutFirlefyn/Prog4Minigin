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
	class TextComponent;
}

class FPSComponent final : public dae::BaseComponent
{
public:
	FPSComponent(dae::GameObject* pGameObject);				// Constructor
	virtual ~FPSComponent() override = default;				// Destructor

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
	virtual void Init() override;
	virtual void Update() override;

private:
	//-------------------------------------------------
	// Private member functions								
	//-------------------------------------------------

	//-------------------------------------------------
	// Datamembers								
	//-------------------------------------------------
	dae::TextComponent* m_pTextComponent{};
	int m_Count{};
	float m_MaxDelay{ 0.1f };
	float m_Delay{};

};


 
