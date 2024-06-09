#include "QbertCurseComponent.h"
#include "QbertComponent.h"
#include "GraphicsComponent.h"
#include "GameTime.h"
#include "GameObject.h"
#include "ServiceLocator.h"
#include "Sounds.h"

QbertCurseComponent::QbertCurseComponent(dae::GameObject* pGameObject, CharacterComponent* pCharacterComponent)
	: BaseComponent(pGameObject)
	, m_pMoveStateChangedSubject{ CharacterComponent::MoveStateChanged.get() }
	, m_Character{ pCharacterComponent->GetCharacter() }
{
}

QbertCurseComponent::~QbertCurseComponent()
{
	if (m_pMoveStateChangedSubject)
		m_pMoveStateChangedSubject->RemoveObserver(this);
}

void QbertCurseComponent::Init()
{
	m_pMoveStateChangedSubject->AddObserver(this);

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

void QbertCurseComponent::Notify(Character character, MovementInfo movementInfo)
{
	if (character != m_Character || movementInfo.state != MovementState::Fall)
		return;

	GetGameObject()->GetComponent<dae::GraphicsComponent>()->ToggleRendering(true);
	dae::ServiceLocator::GetSoundSystem().Play(dae::Sounds::Swearing, 0.2f);
	m_AccumSec = 0.f;
}

void QbertCurseComponent::SubjectDestroyed(dae::Subject<Character, MovementInfo>* pSubject)
{
	if (pSubject == m_pMoveStateChangedSubject)
		m_pMoveStateChangedSubject = nullptr;
}
