#include "LivesComponent.h"
#include "GraphicsComponent.h"
#include "GameObject.h"

int LivesComponent::m_CurrentId{ 0 };

LivesComponent::LivesComponent(dae::GameObject* pGameObject)
	: BaseComponent(pGameObject)
	, m_HeartId{ m_CurrentId++ }
{
}

LivesComponent::~LivesComponent()
{
	if (m_pPlayerDiedSubject)
		m_pPlayerDiedSubject->RemoveObserver(this);
}

void LivesComponent::Init()
{
	m_pPlayerDiedSubject->AddObserver(this);
}

void LivesComponent::AddObserver(dae::Subject<int>* pPlayerDiedSubject)
{
	m_pPlayerDiedSubject = pPlayerDiedSubject;
}

void LivesComponent::Notify(int currentLives)
{
	if (currentLives == m_HeartId)
		GetGameObject()->GetComponent<dae::GraphicsComponent>()->ToggleRendering(false);
}

void LivesComponent::SubjectDestroyed(dae::Subject<int>* pSubject)
{
	if (pSubject == m_pPlayerDiedSubject)
		m_pPlayerDiedSubject = nullptr;
}
