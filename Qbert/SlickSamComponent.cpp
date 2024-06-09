#include "SlickSamComponent.h"
#include "GameObject.h"
#include "GameTime.h"
#include "SpritesheetComponent.h"
#include "GraphicsComponent.h"
#include "ServiceLocator.h"
#include "Sounds.h"
#include "SlickSamStates.h"

SlickSamComponent::SlickSamComponent(dae::GameObject* pGameObject, Character character, LevelManagerComponent* pLevelManagerComponent) 
	: CharacterComponent(pGameObject, pLevelManagerComponent)
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
	if (MoveStateChanged.get())
		MoveStateChanged->RemoveObserver(this);
}

void SlickSamComponent::Init()
{
	MoveStateChanged->AddObserver(this);
	SetState(std::make_unique<SlickSamResetState>(this));
}

void SlickSamComponent::LateUpdate()
{
	if (m_ResetCharacter)
	{
		m_ResetCharacter = false;
		SetState(std::make_unique<SlickSamResetState>(this), false);
	}
}

// MoveStateChanged
void SlickSamComponent::Notify(Character character, MovementInfo movementInfo)
{
	if ((character == Character::Qbert1 || character == Character::Qbert2) && movementInfo.state == MovementState::Fall)
		m_ResetCharacter = true;

	if (character == m_Character && movementInfo.state == MovementState::End)
		dae::ServiceLocator::GetSoundSystem().Play(dae::Sounds::OtherFoesJump, 0.2f);
}
