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
int dae::LivesComponent::m_CurrentId{ 0 };

dae::LivesComponent::LivesComponent(GameObject* pGameObject, QbertComponent* pQbertComponent)
	: BaseComponent(pGameObject)
	, m_pQbertComponent{ pQbertComponent }
	, m_HeartId{ m_CurrentId++ }
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
}

void dae::LivesComponent::Notify()
{
	if (m_pQbertComponent->GetLives() == m_HeartId)
		GetGameObject()->GetComponent<GraphicsComponent>()->ToggleRendering(false);
}

void dae::LivesComponent::SubjectDestroyed(Subject<>* pSubject)
{
	if (pSubject == m_pQbertComponent->PlayerDied.get())
		m_pQbertComponent = nullptr;
}
