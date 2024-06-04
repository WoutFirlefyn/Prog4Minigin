#include "CommandImplementation.h"
#include "GameObject.h"
#include "QbertComponent.h"
#include "CharacterComponent.h"
#include "CoilyComponent.h"
#include <iostream>

MoveCommand::MoveCommand(dae::GameObject* pGameObject, const MovementInfo& movementInfo)
	: GameObjectCommand{ pGameObject }
	, m_pCharacterComponent{ nullptr }
	, m_MovementInfo{ movementInfo }
{
	m_pCharacterComponent = pGameObject->GetComponent<CharacterComponent>();
}

void MoveCommand::Execute()
{
	m_pCharacterComponent->Move(m_MovementInfo);
}