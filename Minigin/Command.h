#pragma once

namespace dae
{
	class Command
	{
	public:
		virtual ~Command() = default;
		virtual void Execute() = 0;
	};

	class GameObject;
	class GameObjectCommand : public Command
	{
	public:
		GameObjectCommand(GameObject* pGameObject)
			: m_pGameObject{ pGameObject }
		{
		}

		virtual ~GameObjectCommand() override = default;

		GameObjectCommand(const GameObjectCommand& other) = delete;
		GameObjectCommand(GameObjectCommand&& other) noexcept = delete;
		GameObjectCommand& operator=(const GameObjectCommand& other) = delete;
		GameObjectCommand& operator=(GameObjectCommand&& other)	noexcept = delete;
	protected:
		GameObject* GetGameObject() const { return m_pGameObject; }
	private:
		GameObject* m_pGameObject;
	};	 

	class MovementComponent;
	class MoveCommand : public GameObjectCommand
	{
	public:
		MoveCommand(GameObject* pGameObject);
	protected:
		MovementComponent* GetMovementComponent();
	private:
		MovementComponent* m_pMovementComponent;
	};

#pragma region MovementDirectionCommands
	class MoveUpCommand : public MoveCommand
	{
	public:
		MoveUpCommand(GameObject* pGameObject) : MoveCommand(pGameObject) {}
		virtual void Execute() override;
	};

	class MoveDownCommand : public MoveCommand
	{
	public:
		MoveDownCommand(GameObject* pGameObject) : MoveCommand(pGameObject) {}
		virtual void Execute() override;
	};

	class MoveLeftCommand : public MoveCommand
	{
	public:
		MoveLeftCommand(GameObject* pGameObject) : MoveCommand(pGameObject) {}
		virtual void Execute() override;
	};

	class MoveRightCommand : public MoveCommand
	{
	public:
		MoveRightCommand(GameObject* pGameObject) : MoveCommand(pGameObject) {}
		virtual void Execute() override;
	};
#pragma endregion
}



