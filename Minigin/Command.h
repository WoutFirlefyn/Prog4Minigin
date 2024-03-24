#pragma once
#include <glm/glm.hpp>

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

	class MoveCommand : public GameObjectCommand
	{
	public:
		MoveCommand(GameObject* pGameObject, const glm::vec3& direction)
			: GameObjectCommand{ pGameObject }
			, m_Direction{ direction }
		{
		}

		virtual void Execute() override;
	private:
		glm::vec3 m_Direction;
	};

	class KillCommand : public GameObjectCommand
	{
	public:
		KillCommand(GameObject* pGameObject)
			: GameObjectCommand{ pGameObject }
		{
		}

		virtual void Execute() override;
	};

	enum class ScoreType;
	class ScoreCommand : public GameObjectCommand
	{
	public:
		ScoreCommand(GameObject* pGameObject, ScoreType scoreType)
			: GameObjectCommand{ pGameObject }
			, m_ScoreType{ scoreType }
		{
		}

		virtual void Execute() override;
	private:
		ScoreType m_ScoreType{};
	};
}

