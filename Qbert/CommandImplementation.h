#pragma once
#include "Command.h"

class CharacterComponent;
enum class MovementDirection;
class MoveCommand : public dae::GameObjectCommand
{
public:
	MoveCommand(dae::GameObject* pGameObject, MovementDirection movementDirection);

	virtual void Execute() override;
private:
	// added m_pCharacterComponent to avoid having to get it every time MoveCommand is executed (which could be a lot if the player is holding down the movementkey)
	CharacterComponent* m_pCharacterComponent{ nullptr };
	MovementDirection m_Direction{};
};

