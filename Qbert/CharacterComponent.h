#pragma once
#include "BaseComponent.h"
#include "Observer.h"
#include <glm/glm.hpp>
#include <unordered_map>

enum class MovementState
{
	Start,
	End,
	Fall,
	Disk
};

enum class MovementDirection
{
	Up = 0,
	Left = 1,
	Right = 2,
	Down = 3,
	DiagonalRight = 12,
	DiagonalLeft = 13,
	None = -1
};

enum class Character
{
	Qbert1 = 0,
	Qbert2 = 1,
	Coily = 2,
	Ugg = 3,
	Wrongway = 4,
	Slick = 5,
	Sam = 6,
	None = -1
};

struct MovementInfo
{
	MovementInfo() = default;

	MovementDirection direction{ MovementDirection::None };
	glm::vec3 vector{ glm::vec3(0) };
	glm::ivec2 indexOffset{ 0,0 };
	MovementState state{ MovementState::Start };

	static void AddMovementInfo(MovementDirection dir, const glm::vec3& vec, const glm::ivec2& offset)
	{
		m_MovementInfos[dir] = MovementInfo(dir, vec, offset);
	}

	static MovementInfo GetMovementInfo(MovementDirection dir)
	{
		auto movementInfoIt = m_MovementInfos.find(dir);
		if (movementInfoIt == m_MovementInfos.end())
			return MovementInfo{};
		return movementInfoIt->second;
	}

	//static MovementInfo GetMovementInfo(const glm::ivec2& offset)
	//{
	//	auto it = std::find_if(m_MovementInfos.begin(), m_MovementInfos.end(),
	//		[&offset](const auto& pair)
	//		{
	//			return pair.second.indexOffset == offset;
	//		});

	//	if (it == m_MovementInfos.end())
	//		return MovementInfo();

	//	return it->second;
	//}
private:
	MovementInfo(MovementDirection dir, const glm::vec3& vec, const glm::ivec2& offset)
		: direction(dir)
		, vector(vec)
		, indexOffset(offset)
	{}

	inline static std::unordered_map<MovementDirection, MovementInfo> m_MovementInfos{};
};

enum class TileType;
class CharacterState;
class LevelManagerComponent;
class CharacterComponent : public dae::BaseComponent, public dae::Observer<bool>
{
public:
	CharacterComponent(dae::GameObject* pGameObject, LevelManagerComponent* pLevelManagerComponent);
	virtual ~CharacterComponent() override;		

	CharacterComponent(const CharacterComponent& other) = delete;
	CharacterComponent(CharacterComponent&& other) noexcept = delete;
	CharacterComponent& operator=(const CharacterComponent& other) = delete;
	CharacterComponent& operator=(CharacterComponent&& other) noexcept = delete;

	virtual void Update() override;

	// TileChanged
	virtual void Notify(bool roundFinished) override;
	virtual void SubjectDestroyed(dae::Subject<bool>* pSubject) override;

	void Move(MovementInfo movementInfo);

	Character GetCharacter() const { return m_Character; }

	virtual glm::ivec2 GetSpawnPosition() const { return m_vSpawnPositions[rand() % m_vSpawnPositions.size()]; }

	static std::unique_ptr<dae::Subject<Character, MovementInfo>> MoveStateChanged;
	static std::unique_ptr<dae::Subject<Character, dae::GameObject*>> CharacterSpawned;
protected:
	Character m_Character{ Character::None };
	std::vector<glm::ivec2> m_vSpawnPositions{};
	bool m_ResetCharacter{ false };

	friend class CharacterState;
	void SetState(std::unique_ptr<CharacterState>&& pNewState, bool callOnExit = true);
	LevelManagerComponent* GetLevelManagerComponent() const { return m_pLevelManagerComponent; };
private:
	std::unique_ptr<CharacterState> m_pState{ nullptr };
	LevelManagerComponent* m_pLevelManagerComponent{ nullptr };
	dae::Subject<bool>* m_pTileChangedSubject{ nullptr };
};

