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
QbertCurseComponent::QbertCurseComponent(dae::GameObject* pGameObject) : BaseComponent(pGameObject)
{
}

QbertCurseComponent::~QbertCurseComponent()
{
	if (m_pPlayerDied)
		m_pPlayerDied->RemoveObserver(this);
}

void QbertCurseComponent::Init()
{
	m_pPlayerDied->AddObserver(this);
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

void QbertCurseComponent::AddObserver(dae::Subject<int>* pPlayerDiedSubject)
{
	m_pPlayerDied = pPlayerDiedSubject;
}

void QbertCurseComponent::Notify(int)
{
	GetGameObject()->GetComponent<dae::GraphicsComponent>()->ToggleRendering(true);
	m_AccumSec = 0.f;
}

void QbertCurseComponent::SubjectDestroyed(dae::Subject<int>* pSubject)
{
	if (pSubject == m_pPlayerDied)
		m_pPlayerDied = nullptr;
}




