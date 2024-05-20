#include "QbertCurseComponent.h"
#include "GraphicsComponent.h"
#include "GameTime.h"
#include "GameObject.h"
#include "ServiceLocator.h"
#include "Sounds.h"

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

	GetGameObject()->SetPosition(m_PosOffset * GetGameObject()->GetWorldScale());
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

void QbertCurseComponent::AddObserver(dae::Subject<>* pPlayerDiedSubject)
{
	m_pPlayerDied = pPlayerDiedSubject;
}

void QbertCurseComponent::Notify()
{
	QbertDied();
}

void QbertCurseComponent::SubjectDestroyed(dae::Subject<>* pSubject)
{
	if (pSubject == m_pPlayerDied)
		m_pPlayerDied = nullptr;
}

void QbertCurseComponent::QbertDied()
{
	GetGameObject()->GetComponent<dae::GraphicsComponent>()->ToggleRendering(true);
	dae::ServiceLocator::GetSoundSystem().Play(dae::Sounds::Swearing, 0.2f);
	m_AccumSec = 0.f;
}
