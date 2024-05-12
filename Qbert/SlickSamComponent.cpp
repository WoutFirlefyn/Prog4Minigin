#include "SlickSamComponent.h"
#include "GameObject.h"
#include "GameTime.h"
#include "ServiceLocator.h"
#include "SpritesheetComponent.h"
#include "GraphicsComponent.h"
#include "Sounds.h"
#include "EnemyStates.h"


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
	SetState(std::make_unique<EnemySpawnState>(this));
}

// MoveStateChanged
void SlickSamComponent::Notify(Character character, MovementState movementState, MovementDirection movementDirection)
{
	if (character != m_Character)
		return;

	switch (movementState)
	{
	case MovementState::Start:
		GetGameObject()->GetComponent<dae::SpritesheetComponent>()->MoveSourceRect(static_cast<int>(movementDirection) - 2, static_cast<int>(m_Character) - static_cast<int>(Character::Slick));
		break;
	case MovementState::End:
		dae::ServiceLocator::GetSoundSystem().Play(dae::Sounds::OtherFoesJump, 0.2f);
		break;
	case MovementState::Fall:
		GetGameObject()->GetComponent<dae::GraphicsComponent>()->ToggleRendering(false);
		break;
	default:
		break;
	}
}

// CharacterSpawned
void SlickSamComponent::Notify(Character character)
{
	if (m_Character != character)
		return;
	CharacterComponent::Notify(character);

	GetGameObject()->GetComponent<dae::SpritesheetComponent>()->MoveSourceRect(0, static_cast<int>(m_Character) - static_cast<int>(Character::Slick));
}
