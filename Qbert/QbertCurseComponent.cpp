//---------------------------
// Includes
//---------------------------
#include "QbertCurseComponent.h"
#include "QbertComponent.h"
#include "GraphicsComponent.h"
#include "GameTime.h"
#include "GameObject.h"

//---------------------------
// Constructor & Destructor
//---------------------------
QbertCurseComponent::QbertCurseComponent(dae::GameObject* pGameObject, QbertComponent* pQbertComponent) : BaseComponent(pGameObject)
	, m_pQbertComponent{ pQbertComponent }
{
}

QbertCurseComponent::~QbertCurseComponent()
{
	if (m_pQbertComponent)
		m_pQbertComponent->MoveStateChanged->RemoveObserver(this);
}

void QbertCurseComponent::Init()
{
	m_pQbertComponent->MoveStateChanged->AddObserver(this);
}

void QbertCurseComponent::Update()
{
	if (m_AccumSec < m_CurseDuration)
	{
		m_AccumSec += dae::GameTime::GetInstance().GetDeltaTime();
		if (m_AccumSec >= m_CurseDuration)
			GetGameObject()->GetComponent<dae::GraphicsComponent>()->ToggleRendering(false);
	}
}

void QbertCurseComponent::Notify(Character, MovementState movementState, MovementDirection)
{
	if (movementState != MovementState::Falling)
		return;
	GetGameObject()->GetComponent<dae::GraphicsComponent>()->ToggleRendering(true);
	m_AccumSec = 0.f;
}

void QbertCurseComponent::SubjectDestroyed(dae::Subject<Character, MovementState, MovementDirection>* pSubject)
{
	if (pSubject == m_pQbertComponent->MoveStateChanged.get())
		m_pQbertComponent = nullptr;
}




