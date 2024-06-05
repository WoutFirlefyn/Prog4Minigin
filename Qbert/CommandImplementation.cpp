#include "CommandImplementation.h"
#include "GameObject.h"
#include "CharacterComponent.h"
#include "MainMenuComponent.h"
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
	GetGameObject()->GetComponent<MainMenuComponent>()->ChangeGamemode(m_GamemodeOffset);
}

void SelectGamemodeCommand::Execute()
{
	GetGameObject()->GetComponent<MainMenuComponent>()->SelectGamemode();
}
