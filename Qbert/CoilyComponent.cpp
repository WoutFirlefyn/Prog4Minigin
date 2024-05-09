#include "CoilyComponent.h"
#include "GameObject.h"
#include "GameTime.h"
#include "ServiceLocator.h"
#include "SpritesheetComponent.h"
#include "Sounds.h"
#include "CoilyStates.h"

bool CoilyComponent::m_IsEgg{ true };

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
	m_pState = std::make_unique<CoilyIdleState>(this);
	GetGameObject()->GetComponent<dae::SpritesheetComponent>()->MoveSourceRect(4, 0);
	m_Character = Character::Coily;
}

void CoilyComponent::Notify(Character, MovementState movementState, MovementDirection movementDirection)
{ 
	int spritesheetCol = (m_IsEgg ? 4 : static_cast<int>(movementDirection));

	switch (movementState)
	{
	case MovementState::Start:
		GetGameObject()->GetComponent<dae::SpritesheetComponent>()->MoveSourceRect(spritesheetCol, 1);
		m_MovementDirection = movementDirection;
		//m_JumpLerpValue = 0.f;
		//m_StartPos = GetGameObject()->GetLocalPosition();
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