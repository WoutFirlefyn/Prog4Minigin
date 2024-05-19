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

SlickSamComponent::~SlickSamComponent()
{
	MoveStateChanged->RemoveObserver(this);
}

void SlickSamComponent::Init()
{
	MoveStateChanged->AddObserver(this);
	SetState(std::make_unique<SlickSamSpawnState>(this));
}

// MoveStateChanged
void SlickSamComponent::Notify(Character character, MovementInfo movementInfo)
{
	if (character != m_Character)
		return;

	if (movementInfo.state == MovementState::End)
		dae::ServiceLocator::GetSoundSystem().Play(dae::Sounds::OtherFoesJump, 0.2f);
}