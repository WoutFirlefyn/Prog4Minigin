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
QbertCurseComponent::QbertCurseComponent(dae::GameObject* pGameObject, dae::Subject<Character>* pSubject) : BaseComponent(pGameObject)
	, m_pCharacterFellSubject{ pSubject }
{
}

QbertCurseComponent::~QbertCurseComponent()
{
	if (m_pCharacterFellSubject)
		m_pCharacterFellSubject->RemoveObserver(this);
}

void QbertCurseComponent::Init()
{
	m_pCharacterFellSubject->AddObserver(this);
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

void QbertCurseComponent::Notify(Character character)
{
	if (character != Character::Qbert1)
		return;
	GetGameObject()->GetComponent<dae::GraphicsComponent>()->ToggleRendering(true);
	m_AccumSec = 0.f;
}

void QbertCurseComponent::SubjectDestroyed(dae::Subject<Character>* pSubject)
{
	if (pSubject == m_pCharacterFellSubject)
		m_pCharacterFellSubject = nullptr;
}




