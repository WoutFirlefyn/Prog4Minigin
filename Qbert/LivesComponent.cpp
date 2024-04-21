//---------------------------
// Includes
//---------------------------
#include "LivesComponent.h"
#include "TextComponent.h"
#include "QbertComponent.h"
#include "GraphicsComponent.h"
#include "GameObject.h"

//---------------------------
// Constructor & Destructor
//---------------------------
int LivesComponent::m_CurrentId{ 0 };

LivesComponent::LivesComponent(dae::GameObject* pGameObject, QbertComponent* pQbertComponent)
	: BaseComponent(pGameObject)
	, m_pQbertComponent{ pQbertComponent }
	, m_HeartId{ m_CurrentId++ }
{
}

LivesComponent::~LivesComponent()
{
	if (m_pQbertComponent)
		m_pQbertComponent->PlayerDied->RemoveObserver(this);
}

//---------------------------
// Member functions
//---------------------------

void LivesComponent::Init()
{
	m_pQbertComponent->PlayerDied->AddObserver(this);
}

void LivesComponent::Notify()
{
	if (m_pQbertComponent->GetLives() == m_HeartId)
		GetGameObject()->GetComponent<dae::GraphicsComponent>()->ToggleRendering(false);
}

void LivesComponent::SubjectDestroyed(dae::Subject<>* pSubject)
{
	if (pSubject == m_pQbertComponent->PlayerDied.get())
		m_pQbertComponent = nullptr;
}
