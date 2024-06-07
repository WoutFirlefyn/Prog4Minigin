#include <SDL.h>
#include "CoilyComponent.h"
#include "GameObject.h"
#include "GameTime.h"
#include "ServiceLocator.h"
#include "SpritesheetComponent.h"
#include "GraphicsComponent.h"
#include "Sounds.h"
#include "CoilyStates.h"
#include "CommandImplementation.h"
#include "InputManager.h"

CoilyComponent::CoilyComponent(dae::GameObject* pGameObject, LevelManagerComponent* pLevelManagerComponent)
	: CharacterComponent(pGameObject, pLevelManagerComponent)
{
}

CoilyComponent::~CoilyComponent()
{
	MoveStateChanged->RemoveObserver(this);
	CharacterSpawned->RemoveObserver(this);
}

void CoilyComponent::Init()
{
	m_vSpawnPositions.push_back({ 1,0 });
	m_vSpawnPositions.push_back({ 0,1 });

	MoveStateChanged->AddObserver(this);
	CharacterSpawned->AddObserver(this);
	m_Character = Character::Coily;
	SetState(std::make_unique<CoilyResetState>(this));
}

void CoilyComponent::LateUpdate()
{
	if (m_ResetCharacter)
	{
		m_ResetCharacter = false;
		SetState(std::make_unique<CoilyResetState>(this), false);
	}
}

void CoilyComponent::Notify(Character character, MovementInfo movementInfo)
{
	if ((character == Character::Qbert1 || character == Character::Qbert2) && movementInfo.state == MovementState::Fall)
		m_ResetCharacter = true;

	if (character == m_Character && movementInfo.state == MovementState::End)
	{
		dae::ServiceLocator::GetSoundSystem().Play((m_IsEgg ? dae::Sounds::CoilyEggJump : dae::Sounds::CoilySnakeJump), 0.2f);
		GetGameObject()->GetComponent<dae::SpritesheetComponent>()->MoveSourceRectRelative(0, -1);
		if (++m_AmountOfJumps == 6)
		{
			m_IsEgg = false;
			GetGameObject()->GetComponent<dae::SpritesheetComponent>()->MoveSourceRect(static_cast<int>(movementInfo.direction), 0);
		}
	}
}

void CoilyComponent::Notify(Character character, dae::GameObject*)
{
	if (m_Character != character)
		return;

	m_IsEgg = true;
	m_AmountOfJumps = 0;
}
