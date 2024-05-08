#include "CommandImplementation.h"
#include "GameObject.h"
#include "QbertComponent.h"
#include "CoilyComponent.h"

MoveCommand::MoveCommand(dae::GameObject* pGameObject, MovementDirection movementDirection)
	: GameObjectCommand{ pGameObject }
	, m_pCharacterComponent{ nullptr }
	, m_Direction{ movementDirection }
{
	if (pGameObject->HasComponent<QbertComponent>())
		m_pCharacterComponent = pGameObject->GetComponent<QbertComponent>();
	else if (pGameObject->HasComponent<CoilyComponent>())
		m_pCharacterComponent = pGameObject->GetComponent<CoilyComponent>();

}

void MoveCommand::Execute()
{
	if (!m_pCharacterComponent->IsMoving())
		m_pCharacterComponent->MoveStateChanged->NotifyObservers(Character::Qbert1, MovementState::Start, m_Direction);
}