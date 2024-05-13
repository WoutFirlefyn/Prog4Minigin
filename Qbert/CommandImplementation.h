#pragma once
#include "Command.h"
#include "CharacterComponent.h"

class MoveCommand : public dae::GameObjectCommand
{
public:
	MoveCommand(dae::GameObject* pGameObject, const MovementInfo& movementInfo);

	virtual void Execute() override;
private:
	// added m_pCharacterComponent to avoid having to get it every time MoveCommand is executed (which could be a lot if the player is holding down the movementkey)
	CharacterComponent* m_pCharacterComponent{ nullptr };
	MovementInfo m_MovementInfo;
};

