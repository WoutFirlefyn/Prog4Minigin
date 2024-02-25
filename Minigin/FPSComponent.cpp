//---------------------------
// Includes
//---------------------------
#include "FPSComponent.h"
#include "TextComponent.h"
#include "GameObject.h"

//---------------------------
// Constructor & Destructor
//---------------------------
dae::FPSComponent::FPSComponent(GameObject* pGameObject) : BaseComponent(pGameObject)
{
	// nothing to create
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
	auto textComponent = m_pGameObject->GetComponent<TextComponent>();
	
	if (textComponent != nullptr)
		textComponent->SetText(std::to_string(1.f / deltaTime) + " FPS");
}




