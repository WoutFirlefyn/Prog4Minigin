#include "CommandImplementation.h"
#include "GameObject.h"
#include "QbertComponent.h"

MoveCommand::MoveCommand(dae::GameObject* pGameObject, MovementDirection movementDirection)
	: GameObjectCommand{ pGameObject }
	, m_pQbertComponent{ pGameObject->GetComponent<QbertComponent>() }
	, m_Direction{ movementDirection }
{
}

void MoveCommand::Execute()
{
	if (!m_pQbertComponent->IsMoving())
		m_pQbertComponent->MoveStateChanged->NotifyObservers(Character::Qbert1, MovementState::Start, m_Direction);
}