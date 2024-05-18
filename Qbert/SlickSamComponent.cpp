#include "SlickSamComponent.h"
#include "GameObject.h"
#include "GameTime.h"
#include "SpritesheetComponent.h"
#include "GraphicsComponent.h"
#include "ServiceLocator.h"
#include "Sounds.h"
#include "SlickSamStates.h"

SlickSamComponent::SlickSamComponent(dae::GameObject* pGameObject, Character character) : CharacterComponent(pGameObject)
{
	if (character != Character::Slick && character != Character::Sam)
	{
		assert(false);
		return;
	}
	m_Character = character;
}

void SlickSamComponent::Init()
{
	CharacterComponent::Init();
	SetState(std::make_unique<SlickSamSpawnState>(this));
}

// MoveStateChanged
void SlickSamComponent::Notify(Character character, MovementInfo movementInfo)
{
	if (character != m_Character)
		return;

	switch (movementInfo.state)
	{
	case MovementState::End:
		dae::ServiceLocator::GetSoundSystem().Play(dae::Sounds::OtherFoesJump, 0.2f);
		break;
	default:
		break;
	}
}