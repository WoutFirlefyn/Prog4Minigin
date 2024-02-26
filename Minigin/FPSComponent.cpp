//---------------------------
// Includes
//---------------------------
#include <sstream>
#include <iomanip>
#include "FPSComponent.h"
#include "TextComponent.h"
#include "GameObject.h"

//---------------------------
// Constructor & Destructor
//---------------------------
dae::FPSComponent::FPSComponent(GameObject* pGameObject) : BaseComponent(pGameObject)
{
	m_pTextComponent = GetParent()->GetComponent<TextComponent>();
}

dae::FPSComponent::~FPSComponent()
{
	// nothing to destroy
}

//---------------------------
// Member functions
//---------------------------


void dae::FPSComponent::Render() const
{
}

void dae::FPSComponent::Update(float deltaTime)
{
	std::stringstream fps;

	fps << std::fixed << std::setprecision(1) << (1.f / deltaTime);
	
	if (m_pTextComponent != nullptr)
		m_pTextComponent->SetText(fps.str() + " FPS");
}




