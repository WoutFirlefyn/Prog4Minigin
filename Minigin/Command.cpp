#include "Command.h"
#include "GameObject.h"
#include "QbertComponent.h"
#include "ScoreComponent.h"
#include "Time.h"

dae::MoveCommand::MoveCommand(GameObject* pGameObject, MovementDirection movementDirection)
	: GameObjectCommand{ pGameObject }
	, m_pQbertComponent{ pGameObject->GetComponent<dae::QbertComponent>() }
	, m_Direction{ movementDirection }
{
}

void dae::MoveCommand::Execute()
{
	if (!m_pQbertComponent->IsMoving())
		m_pQbertComponent->MoveStateChanged->NotifyObservers(Characters::Qbert1, MovementState::Start, m_Direction);
}
