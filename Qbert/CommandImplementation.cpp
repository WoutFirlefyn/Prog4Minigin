#include "CommandImplementation.h"
#include "GameObject.h"
#include "CharacterComponent.h"
#include "MainMenuComponent.h"
#include "LevelManagerComponent.h"
#include "ServiceLocator.h"
#include <iostream>

MoveCommand::MoveCommand(dae::GameObject* pGameObject, const MovementInfo& movementInfo)
	: GameObjectCommand(pGameObject)
	, m_pCharacterComponent{ nullptr }
	, m_MovementInfo{ movementInfo }
{
	m_pCharacterComponent = pGameObject->GetComponent<CharacterComponent>();
}

void MoveCommand::Execute()
{
	m_pCharacterComponent->Move(m_MovementInfo);
}

ChangeGamemodeCommand::ChangeGamemodeCommand(dae::GameObject* pGameObject, int gamemodeOffset)
	: GameObjectCommand(pGameObject)
	, m_GamemodeOffset{ gamemodeOffset }
{
}

void ChangeGamemodeCommand::Execute()
{
	if (!GetGameObject()->HasComponent<MainMenuComponent>())
	{
		assert(false);
		return;
	}
	GetGameObject()->GetComponent<MainMenuComponent>()->ChangeGamemode(m_GamemodeOffset);
}

void SelectGamemodeCommand::Execute()
{
	if (!GetGameObject()->HasComponent<MainMenuComponent>())
	{
		assert(false);
		return;
	}
	GetGameObject()->GetComponent<MainMenuComponent>()->SelectGamemode();
}

void NextRoundCommand::Execute()
{
	if (!GetGameObject()->HasComponent<LevelManagerComponent>())
	{
		assert(false);
		return;
	}
	GetGameObject()->GetComponent<LevelManagerComponent>()->SkipRound();
}

void ToggleSoundCommand::Execute()
{
	dae::ServiceLocator::GetSoundSystem().ToggleMute();
}
