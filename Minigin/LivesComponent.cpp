//---------------------------
// Includes
//---------------------------
#include "LivesComponent.h"
#include "TextComponent.h"
#include "QbertComponent.h"
#include "GameObject.h"

//---------------------------
// Constructor & Destructor
//---------------------------
dae::LivesComponent::LivesComponent(GameObject* pGameObject, QbertComponent* pQbertComponent)
	: BaseComponent(pGameObject)
	, m_pQbertComponent{ pQbertComponent }
{
}

dae::LivesComponent::~LivesComponent()
{
	if (m_pQbertComponent)
		m_pQbertComponent->PlayerDied->RemoveObserver(this);
}

//---------------------------
// Member functions
//---------------------------

void dae::LivesComponent::Init()
{
	m_pQbertComponent->PlayerDied->AddObserver(this);
	m_pTextComponent = GetGameObject()->GetComponent<TextComponent>();
	UpdateText();
}

void dae::LivesComponent::Notify()
{
	UpdateText();
}

void dae::LivesComponent::UpdateText()
{
	if (m_pTextComponent)
		m_pTextComponent->SetText("Lives: " + std::to_string(m_pQbertComponent->GetLives()));
}
