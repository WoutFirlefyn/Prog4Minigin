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
	ChangeGamemodeCommand(dae::GameObject* pGameObject, int gamemodeOffset)
		: GameObjectCommand(pGameObject)
		, m_GamemodeOffset{ gamemodeOffset }
	{
	}

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

class NextRoundCommand : public dae::GameObjectCommand
{
public:
	NextRoundCommand(dae::GameObject* pGameObject) : GameObjectCommand(pGameObject) {}

	virtual void Execute() override;
};

class ToggleSoundCommand : public dae::Command
{
public:
	virtual void Execute() override;
};

class ChangeNameCommand : public dae::GameObjectCommand
{
public:
	ChangeNameCommand(dae::GameObject* pGameObject, const glm::ivec2& offset)
		: GameObjectCommand(pGameObject)
		, m_Offset{ offset }
	{
	}
		
	virtual void Execute() override;
private:
	glm::ivec2 m_Offset{};
};

class SaveHighscore : public dae::GameObjectCommand
{
public:
	SaveHighscore(dae::GameObject* pGameObject) : GameObjectCommand(pGameObject) {}
		
	virtual void Execute() override;
};
