//---------------------------
// Includes
//---------------------------
#include "QbertCurseComponent.h"
#include "QbertComponent.h"
#include "GraphicsComponent.h"
#include "Time.h"
#include "GameObject.h"

//---------------------------
// Constructor & Destructor
//---------------------------
dae::QbertCurseComponent::QbertCurseComponent(GameObject* pGameObject, QbertComponent* pQbertComponent) : BaseComponent(pGameObject)
	, m_pQbertComponent{ pQbertComponent }
{
}

dae::QbertCurseComponent::~QbertCurseComponent()
{
	if (m_pQbertComponent)
		m_pQbertComponent->MoveStateChanged->RemoveObserver(this);
}

void dae::QbertCurseComponent::Init()
{
	m_pQbertComponent->MoveStateChanged->AddObserver(this);
}

void dae::QbertCurseComponent::Update()
{
	if (m_AccumSec < m_CurseDuration)
	{
		m_AccumSec += Time::GetInstance().GetDeltaTime();
		if (m_AccumSec >= m_CurseDuration)
			GetGameObject()->GetComponent<GraphicsComponent>()->ToggleRendering(false);
	}
}

void dae::QbertCurseComponent::Notify(Character, MovementState movementState, MovementDirection)
{
	if (movementState != MovementState::Falling)
		return;
	GetGameObject()->GetComponent<GraphicsComponent>()->ToggleRendering(true);
	m_AccumSec = 0.f;
}

void dae::QbertCurseComponent::SubjectDestroyed(Subject<Character, MovementState, MovementDirection>* pSubject)
{
	if (pSubject == m_pQbertComponent->MoveStateChanged.get())
		m_pQbertComponent = nullptr;
}




