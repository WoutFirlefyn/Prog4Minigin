#pragma once

namespace dae
{
	enum class MovementDirection;
	class QbertComponent;
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

	class MoveCommand : public GameObjectCommand
	{
	public:
		MoveCommand(GameObject* pGameObject, MovementDirection movementDirection);

		virtual void Execute() override;
	private:
		// added m_pQbertComponent to avoid having to get it every time MoveCommand is executed (which could be a lot if the player is holding down the movementkey)
		QbertComponent* m_pQbertComponent{ nullptr };
		MovementDirection m_Direction{};
	};
}

