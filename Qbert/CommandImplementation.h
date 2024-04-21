#pragma once
#include "Command.h"

class QbertComponent;
enum class MovementDirection;
class MoveCommand : public dae::GameObjectCommand
{
public:
	MoveCommand(dae::GameObject* pGameObject, MovementDirection movementDirection);

	virtual void Execute() override;
private:
	// added m_pQbertComponent to avoid having to get it every time MoveCommand is executed (which could be a lot if the player is holding down the movementkey)
	QbertComponent* m_pQbertComponent{ nullptr };
	MovementDirection m_Direction{};
};

