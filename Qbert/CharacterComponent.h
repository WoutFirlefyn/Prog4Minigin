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
	Up,
	Left,
	Right,
	Down,
	None
};

enum class Character
{
	Qbert1,
	Qbert2,
	Coily,
	Ugg,
	Wrongway,
	Slick,
	Sam,
	None
};

struct MovementInfo
{
	MovementInfo(MovementDirection dir)
	{
		MovementInfo movementInfo = m_MovementInfos[dir];
		direction = movementInfo.direction;
		vector = movementInfo.vector;
		indexOffset = movementInfo.indexOffset;
	}
	MovementInfo() = default;

	static void AddMovementInfo(MovementDirection dir, const glm::vec3& vec, const std::pair<int, int>& offset)
	{
		m_MovementInfos[dir] = MovementInfo{ dir, vec, offset };
	}

	MovementDirection direction{ MovementDirection::None };
	glm::vec3 vector{ glm::vec3(0) };
	std::pair<int, int> indexOffset{ 0,0 };
	MovementState state{ MovementState::Start };
private:

	MovementInfo(MovementDirection dir, const glm::vec3& vec, const std::pair<int, int>& offset)
		: direction(dir)
		, vector(vec)
		, indexOffset(offset)
	{}
	inline static std::unordered_map<MovementDirection, MovementInfo> m_MovementInfos{};
};

enum class TileType;
class CharacterState;
class CharacterComponent : public dae::BaseComponent, public dae::Observer<Character, MovementInfo>, public dae::Observer<Character>
{
public:
	CharacterComponent(dae::GameObject* pGameObject);	// Constructor
	virtual ~CharacterComponent() override;				// Destructor

	CharacterComponent(const CharacterComponent& other) = delete;
	CharacterComponent(CharacterComponent&& other) noexcept = delete;
	CharacterComponent& operator=(const CharacterComponent& other) = delete;
	CharacterComponent& operator=(CharacterComponent&& other) noexcept = delete;

	virtual void Init() override;
	virtual void Update() override;

	// MoveStateChanged
	virtual void Notify(Character, MovementInfo) = 0;
	// CharacterSpawned
	virtual void Notify(Character);

	void SetState(std::unique_ptr<CharacterState>&& pNewState);

	void Move(MovementInfo movementInfo);

	glm::vec3 GetPosition() const;
	void SetPosition(const glm::vec3& pos);
	Character GetCharacter() const { return m_Character; }

	static std::unique_ptr<dae::Subject<Character, MovementInfo>> MoveStateChanged;
	static std::unique_ptr<dae::Subject<Character>> CharacterSpawned;
	static std::unique_ptr<dae::Subject<Character>> CharacterDied;
protected:
	std::unique_ptr<CharacterState> m_pState{ nullptr };
	Character m_Character{ Character::None };
};

