#include "CoilyComponent.h"
#include "GameObject.h"
#include "GameTime.h"
#include "ServiceLocator.h"
#include "SpritesheetComponent.h"
#include "Sounds.h"

CoilyComponent::CoilyComponent(dae::GameObject* pGameObject) 
	: CharacterComponent(pGameObject)
{
}

CoilyComponent::~CoilyComponent()
{
}

void CoilyComponent::Init()
{
	CharacterComponent::Init();
	GetGameObject()->GetComponent<dae::SpritesheetComponent>()->MoveSourceRect(4, 0);
	m_Character = Character::Coily;
}

void CoilyComponent::Update()
{
	CharacterComponent::Update();

	m_AccumSec += dae::GameTime::GetInstance().GetDeltaTime();

	if (m_AccumSec > m_TimeBetweenJumps)
	{
		MovementDirection direction = static_cast<MovementDirection>(m_IsEgg ? rand() % 2 + 2 : rand() % 4);

		MoveStateChanged->NotifyObservers(Character::Coily, MovementState::Start, direction);
		m_AccumSec = 0.f;
	}
}


void CoilyComponent::Notify(Character character, MovementState movementState, MovementDirection movementDirection)
{ 
	if (character != Character::Coily)
		return;

	int spritesheetCol = (m_IsEgg ? 4 : static_cast<int>(m_MovementDirection));

	switch (movementState)
	{
	case MovementState::Start:
		m_MovementDirection = movementDirection;
		GetGameObject()->GetComponent<dae::SpritesheetComponent>()->MoveSourceRect(spritesheetCol, 1);
		m_JumpLerpValue = 0.f;
		m_StartPos = GetGameObject()->GetLocalPosition();
		break;
	case MovementState::End:
		dae::ServiceLocator::GetSoundSystem().Play(dae::Sounds::CoilyEggJump, 0.2f);
		GetGameObject()->GetComponent<dae::SpritesheetComponent>()->MoveSourceRect(spritesheetCol, 0);
		m_MovementDirection = MovementDirection::None;
		break;
	default:
		break;
	}
}