#pragma once
#include "BaseComponent.h"
#include "Observer.h"
#include "glm/glm.hpp"

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
	MovementInfo(MovementDirection dir, const glm::vec3& vec, const std::pair<int, int>& offset)
		: direction(dir)
		, vector(vec)
		, indexOffset(offset)
	{
	}
	MovementInfo(MovementDirection dir)
		: direction(dir)
	{
		switch (direction)
		{
		case MovementDirection::Up:
			vector = glm::vec3{ 16, -24, 0 };
			indexOffset = { 0,-1 };
			break;
		case MovementDirection::Left:
			vector = glm::vec3{ -16, -24, 0 };
			indexOffset = { -1,0 };
			break;
		case MovementDirection::Right:
			vector = glm::vec3{ 16, 24, 0 };
			indexOffset = { 1,0 };
			break;
		case MovementDirection::Down:
			vector = glm::vec3{ -16, 24, 0 };
			indexOffset = { 0,1 };
			break;
		default:
			break;
		}
	}
	MovementInfo() = default;
	MovementDirection direction{ MovementDirection::None };
	glm::vec3 vector{ glm::vec3(0) };
	std::pair<int, int> indexOffset{ 0,0 };
	MovementState state{ MovementState::Start };
};

enum class TileType;
class CharacterState;
class CharacterComponent : public dae::BaseComponent, public dae::Observer<Character, MovementInfo>, public dae::Observer<Character, TileType>, public dae::Observer<Character>, public dae::Observer<Character, Character>
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
	// CharacterStartedJumping
	virtual void Notify(Character character, TileType tileType) override;
	virtual void SubjectDestroyed(dae::Subject<Character, TileType>* pSubject) override;
	// CharactersCollide
	virtual void Notify(Character, Character) override;
	virtual void SubjectDestroyed(dae::Subject<Character, Character>* pSubject) override;

	void SetState(std::unique_ptr<CharacterState>&& pNewState);

	void Move(MovementInfo movementInfo);
	TileType GetNextTileType() const { return m_NextTileType; }

	glm::vec3 GetPosition() const;
	void SetPosition(const glm::vec3& pos);
	Character GetCharacter() const { return m_Character; }

	static std::unique_ptr<dae::Subject<Character, MovementInfo>> MoveStateChanged;
	static std::unique_ptr<dae::Subject<Character>> CharacterSpawned;
	static std::unique_ptr<dae::Subject<Character>> CharacterDied;
protected:
	std::unique_ptr<CharacterState> m_pState{ nullptr };
	Character m_Character{ Character::None };
private:
	dae::Subject<Character, TileType>* m_pCharacterStartedJumping{ nullptr };
	dae::Subject<Character, Character>* m_pCharactersCollide{ nullptr };
	TileType m_NextTileType{};
};

