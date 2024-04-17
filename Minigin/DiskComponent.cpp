//---------------------------
// Includes
//---------------------------
#include "DiskComponent.h"
#include "QbertComponent.h"

//---------------------------
// Constructor & Destructor
//---------------------------
dae::DiskComponent::DiskComponent(GameObject* pGameObject, QbertComponent* pQbertComponent) : BaseComponent(pGameObject)
	, m_pQbertComponent{ pQbertComponent }
{
}

dae::DiskComponent::~DiskComponent()
{
	if (m_pQbertComponent)
		m_pQbertComponent->MoveStateChanged->RemoveObserver(this);
}

void dae::DiskComponent::Init()
{
	m_pQbertComponent->MoveStateChanged->AddObserver(this);
}

void dae::DiskComponent::Update()
{
}

void dae::DiskComponent::Notify(Character, MovementState movementState, MovementDirection)
{
	if (movementState != MovementState::End)
		return;


}

void dae::DiskComponent::SubjectDestroyed(Subject<Character, MovementState, MovementDirection>* pSubject)
{
	if (pSubject == m_pQbertComponent->MoveStateChanged.get())
		m_pQbertComponent = nullptr;
}




