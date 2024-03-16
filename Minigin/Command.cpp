#include "Command.h"
#include "GameObject.h"
#include "MovementComponent.h"

dae::MoveCommand::MoveCommand(GameObject* pGameObject)
	: GameObjectCommand{ pGameObject }
	, m_pMovementComponent{ nullptr }
{
}

dae::MovementComponent* dae::MoveCommand::GetMovementComponent()
{
	if (m_pMovementComponent == nullptr)
		m_pMovementComponent = GetGameObject()->GetComponent<MovementComponent>();

	return m_pMovementComponent;
}

#pragma region MovementDirectionCommands
void dae::MoveUpCommand::Execute()
{
	auto pMovementComponent{ GetMovementComponent() };
	assert(pMovementComponent && "MoveUpCommand: m_pMovementComponent is nullptr");

	pMovementComponent->Move(MovementDir::Up);
}


void dae::MoveDownCommand::Execute()
{
	auto pMovementComponent{ GetMovementComponent() };
	assert(pMovementComponent && "MoveDownCommand: m_pMovementComponent is nullptr");

	pMovementComponent->Move(MovementDir::Down);
}


void dae::MoveLeftCommand::Execute()
{
	auto pMovementComponent{ GetMovementComponent() };
	assert(pMovementComponent && "MoveLeftCommand: m_pMovementComponent is nullptr");

	pMovementComponent->Move(MovementDir::Left);
}


void dae::MoveRightCommand::Execute()
{
	auto pMovementComponent{ GetMovementComponent() };
	assert(pMovementComponent && "MoveRightCommand: m_pMovementComponent is nullptr");

	pMovementComponent->Move(MovementDir::Right);
}
#pragma endregion
