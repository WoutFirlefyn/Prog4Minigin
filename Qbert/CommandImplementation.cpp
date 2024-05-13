#include "CommandImplementation.h"
#include "GameObject.h"
#include "QbertComponent.h"
#include "CoilyComponent.h"
#include <iostream>

MoveCommand::MoveCommand(dae::GameObject* pGameObject, const MovementInfo& movementInfo)
	: GameObjectCommand{ pGameObject }
	, m_pCharacterComponent{ nullptr }
	, m_MovementInfo{ movementInfo }
{
	if (pGameObject->HasComponent<QbertComponent>())
		m_pCharacterComponent = pGameObject->GetComponent<QbertComponent>();
	else if (pGameObject->HasComponent<CoilyComponent>())
		m_pCharacterComponent = pGameObject->GetComponent<CoilyComponent>();

}

void MoveCommand::Execute()
{
	m_pCharacterComponent->Move(m_MovementInfo);
}