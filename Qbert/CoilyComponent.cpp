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

CoilyComponent::CoilyComponent(dae::GameObject* pGameObject, LevelManagerComponent* pLevelManagerComponent, bool versusModeEnabled)
	: CharacterComponent(pGameObject, pLevelManagerComponent)
	, m_VersusMode{ versusModeEnabled }
{
	//if (m_VersusMode)
	//{
	//	auto& input = dae::InputManager::GetInstance();
	//	input.BindCommand(std::make_unique<MoveCommand>(this, MovementInfo::GetMovementInfo(MovementDirection::Up)), SDL_SCANCODE_UP, dae::InputType::Down, "Level");
	//	input.BindCommand(std::make_unique<MoveCommand>(this, MovementInfo::GetMovementInfo(MovementDirection::Left)), SDL_SCANCODE_LEFT, dae::InputType::Down, "Level");
	//	input.BindCommand(std::make_unique<MoveCommand>(this, MovementInfo::GetMovementInfo(MovementDirection::Down)), SDL_SCANCODE_DOWN, dae::InputType::Down, "Level");
	//	input.BindCommand(std::make_unique<MoveCommand>(this, MovementInfo::GetMovementInfo(MovementDirection::Right)), SDL_SCANCODE_RIGHT, dae::InputType::Down, "Level");
	//}
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
	SetState(std::make_unique<CoilySpawnState>(this));
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
	if (character != m_Character)
		return;

	if (movementInfo.state == MovementState::End)
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
