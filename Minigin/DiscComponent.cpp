//---------------------------
// Includes
//---------------------------
#include "DiscComponent.h"
#include "QbertComponent.h"

//---------------------------
// Constructor & Destructor
//---------------------------
dae::DiscComponent::DiscComponent(GameObject* pGameObject, QbertComponent* pQbertComponent) : BaseComponent(pGameObject)
	, m_pQbertComponent{ pQbertComponent }
{
}

dae::DiscComponent::~DiscComponent()
{
	if (m_pQbertComponent)
		m_pQbertComponent->MoveStateChanged->RemoveObserver(this);
}

void dae::DiscComponent::Init()
{
	m_pQbertComponent->MoveStateChanged->AddObserver(this);
}

void dae::DiscComponent::Update()
{
}

void dae::DiscComponent::Notify(Characters, MovementState movementState, MovementDirection)
{
	if (movementState != MovementState::End)
		return;


}

void dae::DiscComponent::SubjectDestroyed(Subject<Characters, MovementState, MovementDirection>* pSubject)
{
	if (pSubject == m_pQbertComponent->MoveStateChanged.get())
		m_pQbertComponent = nullptr;
}




