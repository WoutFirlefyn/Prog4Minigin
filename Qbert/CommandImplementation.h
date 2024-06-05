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
	MovementInfo m_MovementInfo{};
};

class ChangeGamemodeCommand : public dae::GameObjectCommand
{
public:
	ChangeGamemodeCommand(dae::GameObject* pGameObject, int gamemodeOffset);

	virtual void Execute() override;
private:
	int m_GamemodeOffset{};
};

class SelectGamemodeCommand : public dae::GameObjectCommand
{
public:
	SelectGamemodeCommand(dae::GameObject* pGameObject) : GameObjectCommand(pGameObject) {}

	virtual void Execute() override;
};

