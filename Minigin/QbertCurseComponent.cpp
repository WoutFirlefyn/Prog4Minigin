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
		m_pQbertComponent->PlayerMoveStateChanged->RemoveObserver(this);
}

void dae::QbertCurseComponent::Init()
{
	m_pQbertComponent->PlayerMoveStateChanged->AddObserver(this);
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

void dae::QbertCurseComponent::Notify(MovementState movementState, MovementDirection movementDirection)
{
	(void)movementDirection;
	if (movementState != MovementState::Falling)
		return;
	GetGameObject()->GetComponent<GraphicsComponent>()->ToggleRendering(true);
	m_AccumSec = 0.f;
}

void dae::QbertCurseComponent::SubjectDestroyed(Subject<MovementState, MovementDirection>* pSubject)
{
	if (pSubject == m_pQbertComponent->PlayerMoveStateChanged.get())
		m_pQbertComponent = nullptr;
}




