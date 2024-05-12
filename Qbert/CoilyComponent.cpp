#include "CoilyComponent.h"
#include "GameObject.h"
#include "GameTime.h"
#include "ServiceLocator.h"
#include "SpritesheetComponent.h"
#include "GraphicsComponent.h"
#include "Sounds.h"
#include "CoilyStates.h"

bool CoilyComponent::m_IsEgg{ true };

CoilyComponent::CoilyComponent(dae::GameObject* pGameObject) 
	: CharacterComponent(pGameObject)
{
}

void CoilyComponent::Init()
{
	CharacterComponent::Init();
	m_Character = Character::Coily;
	SetState(std::make_unique<CoilySpawnState>(this));
}

void CoilyComponent::AddObserver(dae::Subject<Character, TileType>*)
{
	//m_pCharacterStartedJumping = pCharacterStartedJumpingSubject;
}

void CoilyComponent::Notify(Character character, MovementState movementState, MovementDirection movementDirection)
{
	if (character != m_Character)
		return;

	int spritesheetCol = (m_IsEgg ? 4 : static_cast<int>(movementDirection));

	switch (movementState)
	{
	case MovementState::Start:
		GetGameObject()->GetComponent<dae::SpritesheetComponent>()->MoveSourceRect(spritesheetCol, 1);
		break;
	case MovementState::End:
		dae::ServiceLocator::GetSoundSystem().Play((m_IsEgg ? dae::Sounds::CoilyEggJump : dae::Sounds::CoilySnakeJump), 0.2f);
		GetGameObject()->GetComponent<dae::SpritesheetComponent>()->MoveSourceRect(spritesheetCol, 0);
		if (++m_AmountOfJumps == 6)
			m_IsEgg = false;
		break;
	case MovementState::Fall:
		dae::ServiceLocator::GetSoundSystem().Play(dae::Sounds::CoilyFall, 0.2f);
		GetGameObject()->GetComponent<dae::GraphicsComponent>()->ToggleRendering(false);
		break;
	default:
		break;
	}
}

void CoilyComponent::Notify(Character character)
{
	if (m_Character != character)
		return;

	CharacterComponent::Notify(character);
	m_IsEgg = true;
	m_AmountOfJumps = 0;
	GetGameObject()->GetComponent<dae::SpritesheetComponent>()->MoveSourceRect(4, 1);
}
